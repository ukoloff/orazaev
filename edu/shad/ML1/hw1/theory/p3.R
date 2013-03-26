# input params:
a = 3
b = 3
s = 25

C = c(300, 0.5)
Var = matrix(c(1, 0, 0, s), 2)
Mu = matrix(c(a, b, -a, -b), 2)

library('mvtnorm')

GenerateData = function(n, Mu, Var) {
  result = data.frame()
  for (i in 1:ncol(Mu)) {
    X = rmvnorm(n, mean=Mu[, i], sigma=Var)
    result = rbind(result, data.frame(X, component=rep(i, n)))
  }

  return (result)
}


set.seed(42)
data = GenerateData(1000, Mu, Var)
plot(data[, c("X1", "X2")], col=data$component)

separationSurface = function(x) {
    return ((s / b) * (log(C[2] / C[1]) / 4 - a * x))
}

lines(c(-1000, 1000), separationSurface(c(-1000, 1000)))
