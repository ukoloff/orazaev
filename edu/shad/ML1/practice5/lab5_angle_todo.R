# Load package "lestat"
library('lestat')

# Implement function to get max and min angles
getMinMaxAngles = function(points, index) {
  angles = data.frame(t(c(Inf, -Inf)))
  names(angles) = c("Min", "Max")

  if (nrow(points) == 1) {
    angles$Min = angles$Max = NaN
    return (angles)
  }

  elem = points[index, ]
  for (i in 1:nrow(points)) {
    if (i != index) {
      cur = points[i, ]
      cur.angle = sum(cur * elem) / (sum(cur * cur) * sum(elem * elem))

      if (cur.angle > angles$Max) {
        angles$Max = cur.angle
      }

      if (cur.angle < angles$Min) {
        angles$Min = cur.angle
      }
    }
  }

  return (angles)
}


dimensions = 1:200
average_min_angles = vector(length=length(dimensions))
average_max_angles = vector(length=length(dimensions))

for (i in 1:length(dimensions)) {
  dim = dimensions[i];
  
  # Generate 100 points from [-1,1]^d 
  # (use functions "muniformdistribution" and "simulate")
  mud = muniformdistribution(rep(-1, dim), rep(1, dim))
  points = simulate(mud, 100)

  min_angles = vector(length=nrow(points))
  max_angles = vector(length=nrow(points))
  for(point in 1:nrow(points)) {
    # for each point find values of max and min cosines
    # ...	
    angles = getMinMaxAngles(points, point)
    min_angles[point] = angles$Min
    max_angles[point] = angles$Max
  }

average_min_angles[i] = mean(min_angles)
average_max_angles[i] = mean(max_angles)

}

answers_min = average_min_angles
answers_max = average_max_angles

plot(answers_min, ylim=c(-1,1), col="red", pch=20, xlab="dim", ylab="min and max")
# par(new=T)
points(answers_max, ylim=c(-1,1), col="blue", pch=20, new=T)
