library('lestat')

dimensions = 1:1000
answersL1 = vector(length=length(dimensions))
answersL2 = vector(length=length(dimensions))
answersLInf = vector(length=length(dimensions))

getNormFunctor = function (p) {
    if (p == Inf) {
        return (function(x) { return (max(abs(x))) })
    }
    return (function(x) { return (sum(abs(x) ^ p) ^ 1/p) })
}

for (i in 1:length(dimensions)) {
  dim = dimensions[i];
  # Generate 1000 points from [-1,1]^d 
  # (use functions "muniformdistribution" and "simulate")
  mud = muniformdistribution(rep(-1, dim), rep(1, dim))
  points = simulate(mud, 100)
  
  # Calculate norms
  norms = apply(points, 1, getNormFunctor(1))
  answersL1[i] = (max(norms) - min(norms))/max(norms)

  norms = apply(points, 1, getNormFunctor(2))
  answersL2[i] = (max(norms) - min(norms))/max(norms)

  norms = apply(points, 1, getNormFunctor(Inf))
  answersLInf[i] = (max(norms) - min(norms))/max(norms)
}

png('norms.png')
plot(answersLInf, type='l', col="red", ylab="answers", xlab="dimension")
lines(answersL2, col="blue")
lines(answersL1, col="green", ylim=c(0, 1))

legendPos = length(dimensions) * 0.75
legend(legendPos, 1, c("L^1", "L^2", "L^Inf"), col=c("green", "blue", "red"), bty="n", lty=1)

dev.off()
