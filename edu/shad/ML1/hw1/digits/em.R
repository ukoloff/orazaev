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
  return (dmvnorm(x, Theta$Mean[i,], diag(Theta$Sigma[i,])))
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
    print(sprintf("EM: chages mean=%f, sigma=%f",
        max(abs(old_mean - Theta$Mean)),
        max(abs(old_sigma - Theta$Sigma))))

    if (max(abs(old_mean - Theta$Mean)) < delta &&
        max(abs(old_sigma - Theta$Sigma)) < delta) break
  }

  return (Theta)
}



GetInitialMeans = function(X, k) {
  return (X[sample(1:nrow(X), k), ])
}

GetInitialVars = function(X, k) {
  return (matrix(1, nrow=k, ncol=ncol(X)))
}

GetInitialWeights = function(X, k) {
  return (rep(1/k, k))
}

GetInitialTheta = function(X, k) {
  return (CreateTheta(GetInitialWeights(X, k),
      GetInitialMeans(X, k),
      GetInitialVars(X, k)))
}

GEM = function(X, R, m0, delta) {
  k = 1
  Theta = EM(X, k, initial_theta, delta)
}
