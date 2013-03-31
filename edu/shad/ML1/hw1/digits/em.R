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
  sigma = as.vector(Theta$Sigma[i,])
  #mu = as.vector(Theta$Mean[i,])[sigma != 0]
  #x = as.vector(x)[sigma != 0]
  #sigma = sigma[sigma != 0]
  mu = as.vector(Theta$Mean[i,])
  return (exp(-0.5 * sum((x - mu)^2 * sigma^(-1))) /
      ((2 * pi) ^ (dim / 2) * sqrt(prod(sigma))))
}



CalcDensity = function(x, Theta) {
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

      #print(Px)
      for (j in 1:k) {
        if (Px == 0) {
            g[i, j] = 0
            next
        }
        g[i, j] = Theta$W[j] * Phi(X[i,], Theta, j) / Px
      }
    }

    #plot(X, col='blue', pch=19)
    #points(Theta$Mean, pch=10, col=1:k, cex=10)


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
    # print(Theta$Mean)
    # print(Theta$Sigma)

    if (any(is.na(old_mean -Theta$Mean)) ||
        any(is.na(old_sigma - Theta$Sigma)) ||
        any(is.na(old_w - Theta$W))) break

    if (max(abs(old_mean - Theta$Mean)) < delta &&
        max(abs(old_sigma - Theta$Sigma)) < delta &&
        max(abs(old_w - Theta$W) < delta)) break
  }

  return (Theta)
}



GetInitialMeans = function(X, k=1) {
  if (k == 1) {
    return (matrix(X[sample(1:nrow(X), 1),], nrow=k))
    return (matrix(colMeans(X), nrow=k))
  }

  return (as.matrix(X[sample(1:nrow(X), k), ]))
}

GetInitialVars = function(X, k=1) {
  if (k == 1) {
    return (matrix(apply(t(X), 1, var), nrow=k))
  }

  return (matrix(1, nrow=k, ncol=ncol(X)))
}

GetInitialWeights = function(X, k=1) {
  return (rep(1 / k, k))
}

GetInitialTheta = function(X, k=1) {
  return (CreateTheta(GetInitialWeights(X, k),
      GetInitialMeans(X, k),
      GetInitialVars(X, k)))
}

GEM = function(X, delta, minK=0) {
  k = 1
  Theta = GetInitialTheta(X)
  if (minK != 0) {
    Theta = GetInitialTheta(X, minK - 1)
    k = minK - 1
  }
  oldTheta = NULL

  #print(Theta)
  repeat {
    pct = proc.time()

    # Choose k using mutual information
    # http://en.wikipedia.org/wiki/Mutual_information
    # http://www.google.ru/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0CDEQFjAA&url=http%3A%2F%2Fciteseerx.ist.psu.edu%2Fviewdoc%2Fdownload%3Fdoi%3D10.1.1.109.8192%26rep%3Drep1%26type%3Dpdf&ei=oXtTUeqCBcaS4ATEyoB4&usg=AFQjCNFXjHdfDggeWS7WSAB5EAB731oluw&bvm=bv.44342787,d.bGE&cad=rjt
    #
    if (!is.null(oldTheta)) {
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
      if (any(is.na(mutualInformation)) || (max(mutualInformation) >= 7 || k >= 7)) {
        break
      }
    }

    dens = apply(X, 1, function(x) { return (CalcDensity(x, Theta)) })
    #print(dens) # dens is NAN need to fix.
    if (any(is.na(dens))) break
    minIndex = which.min(dens)

    # Update Theta
    oldTheta = Theta
    k = k + 1
    U = dens < sort(dens)[0.3 * length(dens)]
    t = GetInitialTheta(X[U,])
    #print(apply(X, 1, function(x) { return (Phi(x, Theta, 1)) }))
    #print(apply(X, 1, function(x) { return (Phi(x, t, 1)) }))
    #Theta$Mean = rbind(Theta$Mean, X[minIndex,])
    #Theta$Sigma = rbind(Theta$Sigma, rep(0.25, ncol(X)))
    Theta$Mean = rbind(Theta$Mean , t$Mean)
    Theta$Sigma = rbind(Theta$Sigma, t$Sigma)
    Theta$W = Theta$W * (1 - 1 / sum(U))
    Theta$W = c(Theta$W, 1 / sum(U))

    print("Time to calculate initial theta: ")
    print(proc.time() - pct)
    pct = proc.time()
    Theta = EM(X, k, Theta, delta)
    print("EM algorithm time: ")
    print(proc.time() - pct)
  }

  return (oldTheta)
}
