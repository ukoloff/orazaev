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
        X = (X - rep(1, nrow(X)) %*% t(new.means)) /
            (rep(1, nrow(X)) %*% t(sqrt(new.vars)))

        return(X)
    }

    return (preprocessing)
}
