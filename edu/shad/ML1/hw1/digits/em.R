# EM-algorithm realization.
#
# Copyright 2013 Aman Orazaev

library('mvtnorm')

# Here and below Theta is the list of:
#   * Theta$W -- vector of weights of components
#   * Theta$Mean -- Matrix of means of components
#   * Theta$Sigma -- Matrix of varience diagonal values of components



Phi = function(x, Theta, i) {
  return (dmvnorm(x, Theta$Mean[i,], diag(Theta$Sigma[i,])))
}



EM = function(X, k, Theta, delta) {
  g = matrix(Inf, nrow=nrow(X), ncol=k)

  repeat {
    old_g = g
    old_mean = Theta$Mean

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

    print(max(abs(old_g - g)))
    print("Hello!")
    print(g[1, ])
    # if (max(abs(old_g - g)) < delta) break
    if (max(abs(old_mean - Theta$Mean)) < delta) break
  }

  return (Theta)
}
