library('MASS')

NORMALIZE_THREASHOLD = 0.001 ^ 2

colVars = function(X) {
    return (apply(t(X), 1, var))
}

calcMvnormDensity = function(X, Mean, Var) {
    if (is.vector(X)) {
        X = t(as.matrix(X))
    }
    X = as.matrix(X)
    d = ncol(X)

    PDF = function(x) {
        return (
            exp(-0.5 * sum((x - Mean)^2 / Var)) /
            sqrt((2 * pi)^d * prod(Var))
        )
    }

    return (t(apply(X, 1, PDF)))
}

calcNativeBayesDensity = function(X, Mean, Var) {
    if (is.vector(X)) {
        X = t(as.matrix(X))
    }
    X = as.matrix(X)
    PDF = function(x) {
        return (
            exp(-0.5 * (x - Mean)^2 / Var) /
            (sqrt(2 * pi * Var))
        )
    }
    return (t(apply(X, 1, PDF)))
}

normalizeSingularity = function(X) {
    vars = colVars(X)
    return (X[, vars > NORMALIZE_THREASHOLD])
}

normalizeScale = function(X) {
    maxs = apply(t(X), 1, max)
    mins = apply(t(X), 1, min)

    return (
        (X - rep(1, nrow(X)) %*% t(mins)) /
        (rep(1, nrow(X)) %*% t(maxs - mins))
    )
}

normalizeNorm = function(X) {
    vars = colVars(X)
    means = colMeans(X)

    return (
        (X - rep(1, nrow(X)) %*% t(means)) /
        (rep(1, nrow(X)) %*% t(sqrt(vars)))
    )
}

getPca = function(X) {
    pca = list()

    Sigma = (t(as.matrix(X)) %*% as.matrix(X)) / nrow(X)
    pca$svd = svd(Sigma)

    pca$Reduce = function(X, new.dim) {
        return (as.matrix(X) %*% pca$svd$u[, 1:new.dim])
    }

    pca$Recover = function(Z) {
        return (as.matrix(Z) %*% t(pca$svd$u[, 1:ncol(Z)]))
    }

    return (pca)
}

make.matrix = function(X) {
    if (is.vector(X)) {
        return (t(as.matrix(X)))
    }
    return (as.matrix(X))
}

getPreprocessing = function(X, reduced.dim=60) {
    preprocessing = list()
    X = as.matrix(X)

    vars = colVars(X)
    non.zero.vars = vars > NORMALIZE_THREASHOLD

    means = colMeans(X[, non.zero.vars])
    vars = vars[non.zero.vars]

    pca = getPca(X[, non.zero.vars])
    new.X = pca$Reduce(X[, non.zero.vars], reduced.dim)
    new.means = colMeans(new.X)
    new.vars = colVars(new.X)

    preprocessing$Do = function(X) {
        X = make.matrix(X)

        X = make.matrix(X[, non.zero.vars])
        X = make.matrix((X - rep(1, nrow(X)) %*% t(means)) /
            (rep(1, nrow(X)) %*% t(sqrt(vars))))

        X = make.matrix(pca$Reduce(X, reduced.dim))

        return(X)
    }

    return (preprocessing)
}

shuffle = function(X) {
    return (X[sample(1:nrow(X), nrow(X)), ])
}

prepareData = function(X, train.factor=0.6) {
    y = X[, ncol(X)]

    data = list()
    data$train = data.frame()
    data$test = data.frame()

    for (label in unique(y)) {
        curLabelData = X[y == label, ]
        curSize = nrow(curLabelData)
        lastTestIndex = train.factor * curSize

        data$train = rbind(data$train, curLabelData[1:lastTestIndex, ])
        data$test = rbind(data$test, curLabelData[(lastTestIndex + 1):curSize, ])
    }

    return (data)
}

trainLda = function(X) {
    lda = list()

    y = X[, ncol(X)]
    X = X[, -ncol(X)]
    X = as.matrix(X)

    trainSize = nrow(X)

    vars = colVars(X)
    lda$non.zero.vars = vars > NORMALIZE_THREASHOLD
    X = X[, lda$non.zero.vars]

    normalize.means = colMeans(X)
    normalize.vars = colVars(X)
    normalize = function(x) {
        return (
            (x - normalize.means) / sqrt(normalize.vars)
        )
    }
    X = t(apply(X, 1, normalize))
    pca = getPca(X)
    X = pca$Reduce(X, 279)


    lda$means = list()
    lda$prob = list()
    lda$label = c()
    lda$Sigma = matrix(0, nrow=ncol(X), ncol=ncol(X))

    for (label in unique(y)) {
        curLabelData = X[y == label, ]
        lda$means[[length(lda$means) + 1]] = colMeans(curLabelData)
        lda$prob[[length(lda$prob) + 1]] = nrow(curLabelData) / trainSize
        lda$label = c(lda$label, label)
    }

    for (i in 1:trainSize) {
        index = which(lda$label == y[i])

        centred = X[i, ] - lda$means[[index]]
        lda$Sigma = lda$Sigma + (as.matrix(centred) %*%
                    t(as.matrix(centred))) / trainSize

        print(sprintf("Training [%.2f%%]", i * 100 / trainSize))
    }

    lda$SigmaInv = ginv(lda$Sigma)
    lda$aTerm == list()
    lda$bTerm = list()

    for (index in 1:length(lda$label)) {
        lda$bTerm[[index]] = log(lda$prob[[index]]) - (make.matrix(lda$means[[index]]) %*%
                             lda$SigmaInv %*% as.matrix(lda$means[[index]]))
        lda$aTerm[[index]] = lda$SigmaInv %*% as.matrix(lda$means[[index]])
    }

    lda$Classify = function(Z) {
        Z = make.matrix(Z);
        Z = make.matrix(Z[, lda$non.zero.vars])
        Z = make.matrix(t(apply(Z, 1, normalize)))
        Z = pca$Reduce(Z, 279)
        Score = data.frame();

        for (index in 1:length(lda$label)) {
            F = function(x) {
                return (
                    t(as.matrix(x)) %*% lda$aTerm[[index]] + lda$bTerm[[index]]
                )
            }

            Score = rbind(Score, apply(Z, 1, F))
        }

        return (lda$label[apply(t(Score), 1, which.max)])
    }

    return (lda)
}
