source('em.R')
library('mvtnorm')

X = rmvnorm(500, mean=c(5, 5, 5), sigma=rbind(c(2, 0.3, 0.1), c(0.3, 0.2, 0.1), c(0.1, 0.1, 1)))
X = rbind(X, rmvnorm(500, mean=c(-5, 0, 0)))
X = rbind(X, rmvnorm(500, mean=c(0, 0, 0), sigma=diag(c(5, 5, 5))))

mins = apply(t(X), 1, min)
maxs = apply(t(X), 1, max)

X = X - rep(1, nrow(X)) %*% t(mins)
X = X / rep(1, nrow(X)) %*% t(maxs - mins)

Theta = GEM(X, 0.001)

k = nrow(Theta$Mean)
plot(X, col='blue', pch=19)

for (i in 1:k) {
  P = rmvnorm(round(2000 * Theta$W[i]), mean=Theta$Mean[i,], sigma=diag(Theta$Sigma[i,]))
  points(P, col=i, cex=0.8, pch=4)
}
points(Theta$Mean, pch=10, col=1:k, cex=10)
