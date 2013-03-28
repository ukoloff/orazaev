# EM algorithm demonstration

irisData = datasets::iris

X = as.matrix(irisData[irisData$Species == "setosa", -5])

source("em.R")

k = 4
delta = 0.001
R = 2
m0 = 5

#Theta = EM(X, k, GetInitialTheta(X, k), delta)
Theta = GEM(X, delta)
k = nrow(Theta$Mean)

plot(X, col='blue', pch=19)

for (i in 1:k) {
  P = rmvnorm(round(2000 * Theta$W[i]), mean=Theta$Mean[i,], sigma=diag(Theta$Sigma[i,]))
  points(P, col=i, cex=0.8, pch=4)
}
points(Theta$Mean, pch=10, col=1:k, cex=10)
