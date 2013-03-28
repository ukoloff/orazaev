# EM-algorithm realization.
#
# Copyright 2013 Aman Orazaev

library('mvtnorm')

# Here and below Theta is the list of:
#   * Theta$W -- vector of weights of components
#   * Theta$Mean -- Matrix of means of components
#   * Theta$Sigma -- Matrix of varience diagonal values of components
CreateTheta = function(weights, means, vars) {
  Theta = list(weights, means, vars)
  names(Theta) = c("W", "Mean", "Sigma")

  return (Theta)
}



Phi = function(x, Theta, i) {
  # very slow box implementation
  # return (dmvnorm(x, Theta$Mean[i,], diag(Theta$Sigma[i,])))
  dim = nrow(Theta$Mean)
  sigma = Theta$Sigma[i,]
  mu = Theta$Mean[i,]
  return (exp(-0.5 * (x - mu) %*% diag(sigma^(-1)) %*% (x - mu)) /
      ((2 * pi) ^ (dim / 2) * sqrt(prod(sigma))))
}



CalcDencity = function(x, Theta) {
  dencity = 0
  for (i in 1:nrow(Theta$Mean)) {
    dencity = dencity + Theta$W[i] * Phi(x, Theta, i)
  }

  return (dencity)
}



EM = function(X, k, Theta, delta) {
  g = matrix(Inf, nrow=nrow(X), ncol=k)

  repeat {
    old_g = g
    old_mean = Theta$Mean
    old_sigma = Theta$Sigma
    old_w = Theta$W

    # E-step
    for (i in 1:nrow(X)) {
      # Calculating P(x)
      Px = 0
      for (j in 1:k) {
        Px = Px + Theta$W[j] * Phi(X[i,], Theta, j)
      }

      for (j in 1:k) {
        g[i, j] = Theta$W[j] * Phi(X[i,], Theta, j) / Px
      }
    }

    plot(X, col='blue', pch=19)
    points(Theta$Mean, pch=10, col=1:k, cex=10)

    # M-step
    Theta$W = apply(t(g), 1, sum) / nrow(X)
    Theta$Mean = (t(g) %*% X)
    Theta$Mean = Theta$Mean / (nrow(X) * Theta$W %*% t(rep(1, ncol(X))))

    for (j in 1:k)
    for (d in 1:ncol(X)) {
      Theta$Sigma[j, d] = (t(g[,j]) %*% (X[, d] - Theta$Mean[j, d]) ^ 2) /
                          (nrow(X) * Theta$W[j])
    }


    # delta
    print(sprintf("EM(%d): chages mean=%f, sigma=%f", k,
        max(abs(old_mean - Theta$Mean)),
        max(abs(old_sigma - Theta$Sigma))))

    if (max(abs(old_mean - Theta$Mean)) < delta &&
        max(abs(old_sigma - Theta$Sigma)) < delta &&
        max(abs(old_w - Theta$W) < delta)) break
  }

  return (Theta)
}



GetInitialMeans = function(X, k=1) {
  if (k == 1) {
    return (matrix(colMeans(X), nrow=k))
  }

  return (X[sample(1:nrow(X), k), ])
}

GetInitialVars = function(X, k=1) {
  if (k == 1) {
    return (matrix(apply(t(X), 1, var), nrow=k))
  }

  return (matrix(1, nrow=k, ncol=ncol(X)))
}

GetInitialWeights = function(X, k=1) {
  print(k)
  return (rep(1 / k, k))
}

GetInitialTheta = function(X, k=1) {
  return (CreateTheta(GetInitialWeights(X, k),
      GetInitialMeans(X, k),
      GetInitialVars(X, k)))
}

GEM = function(X, delta) {
  k = 1
  Theta = GetInitialTheta(X)
  oldTheta = NULL

  repeat {
    pct = proc.time()

    # Choose k using mutual information
    # http://en.wikipedia.org/wiki/Mutual_information
    # http://www.google.ru/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0CDEQFjAA&url=http%3A%2F%2Fciteseerx.ist.psu.edu%2Fviewdoc%2Fdownload%3Fdoi%3D10.1.1.109.8192%26rep%3Drep1%26type%3Dpdf&ei=oXtTUeqCBcaS4ATEyoB4&usg=AFQjCNFXjHdfDggeWS7WSAB5EAB731oluw&bvm=bv.44342787,d.bGE&cad=rjt
    #
    if (k != 1) {
      mutualInformation = matrix(-Inf, nrow=k, ncol=k)
      for (i in 1:(k - 1)) {
      for (j in (i + 1):k) {
        Pi = apply(X, 1, function(x) { return (Phi(x, Theta, i)) })
        Pj = apply(X, 1, function(x) { return (Phi(x, Theta, j)) })

        mutualInformation[i, j] = nrow(X) * sum(Pi * Pj) / (sum(Pi) * sum(Pj))
        mutualInformation[i, j] = log(mutualInformation[i, j])
      }
      }

      print(mutualInformation)
      if (any(is.na(mutualInformation)) || max(mutualInformation) >= 0) {
        break
      }
    }

    dens = apply(X, 1, function(x) { return (CalcDencity(x, Theta)) })
    minIndex = which.min(dens)

    # Update Theta
    oldTheta = Theta
    k = k + 1
    Theta$Mean = rbind(Theta$Mean, X[minIndex,])
    Theta$Sigma = rbind(Theta$Sigma, rep(1, ncol(X)))
    Theta$W = Theta$W * (1 - 1 / k)
    Theta$W = c(Theta$W, 1 / k)

    print("Time to calculate initial theta: ")
    print(proc.time() - pct)
    pct = proc.time()
    Theta = EM(X, k, Theta, delta)
    print("EM algorithm time: ")
    print(proc.time() - pct)
  }

  print(oldTheta)
  return (oldTheta)
}
