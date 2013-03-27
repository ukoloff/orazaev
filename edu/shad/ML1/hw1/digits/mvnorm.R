source('em.R')
library('mvtnorm')

X = rmvnorm(300, mean=c(5, 0), sigma=rbind(c(2, 0.3), c(0.3, 0.2)))
X = rbind(X, rmvnorm(300, mean=c(-5, 0)))

Theta = GEM(X, 0.05)

k = nrow(Theta$Mean)
plot(X, col='blue', pch=19)

for (i in 1:k) {
  P = rmvnorm(round(2000 * Theta$W[i]), mean=Theta$Mean[i,], sigma=diag(Theta$Sigma[i,]))
  points(P, col=i, cex=0.8, pch=4)
}
points(Theta$Mean, pch=10, col=1:k, cex=10)
