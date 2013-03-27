# EM algorithm demonstration

irisData = datasets::iris

X = as.matrix(irisData[irisData$Species == "setosa", c(1, 2)])

source("em.R")

k = 4
delta = 0.005
initial_mean = matrix(c(X[1:k,]), nrow=k)
initial_var = matrix(1, nrow=k, ncol=ncol(initial_mean))
initial_weights = rep(1/k, k)

initial_theta = list(initial_weights, initial_mean, initial_var)
names(initial_theta) = c("W", "Mean", "Sigma")

Theta = EM(X, k, initial_theta, delta)

plot(X, col='blue', pch=19)

for (i in 1:k) {
  P = rmvnorm(round(2000 * Theta$W[i]), mean=Theta$Mean[i,], sigma=diag(Theta$Sigma[i,]))
  points(P, col=i, cex=0.8, pch=4)
}
points(Theta$Mean, pch=10, col=1:k, cex=10)
