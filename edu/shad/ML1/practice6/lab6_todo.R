# read train and test data
train_data = read.csv('train.csv')
test_data = read.csv('test.csv')

dim = ncol(train_data)

set.seed(13)

# initialize weights randomly from (\frac{-1}{2n}, \frac{1}{2n})
weights = runif(dim, min = -0.5 / dim, max = 0.5 / dim)
#weights = rep(0, dim)

getNormalizer = function(data) {
  data = data[, -dim]
  normalizer = list()
  normalizer$Vars = sqrt(apply(t(data), 1, var))
  normalizer$Means = colMeans(data)

  normalizer$Normalize = function(x) {
    x = as.matrix(x)
    x = x - rep(1, nrow(x)) %*% t(normalizer$Means)
    x = x / (rep(1, nrow(x)) %*% t(normalizer$Vars))
    return (x)
  }

  return (normalizer)
}

normalizer = getNormalizer(train_data)

calcMargin = function(data, weights) {
  y = data[, dim]
  data = as.matrix(data[, -dim])
  data = normalizer$Normalize(data)
  data = cbind(rep(-1, nrow(data)), data)

  return(as.numeric(data %*% weights * y))
}

#implement function for fast computing of errors rate
getError = function(data, weights) {
  y = data[, dim]
  data = as.matrix(data[, -dim])
  data = normalizer$Normalize(data)
  data = cbind(rep(-1, nrow(data)), data)

  prediction = data %*% weights

  errors_count = sum((prediction * y) <= 0)
  return (errors_count / nrow(data))
}



# implement Hubb's rule with fixed learning rate

calcWeights = function(train_data, test_data, initial, iters=500, rate=0.009, draw=F) {
  learning_rate = rate
  iterations_count = iters
  train_errors = 0 * 1:iterations_count
  test_errors = 0 * 1:iterations_count

  weights = initial
  good_weights = NA
  min_error = 1

  prev_y = 0
  y = 0


  for(i in 1:iterations_count) {
    while (y == prev_y) {
      r_number = sample(1:nrow(train_data), 1)
      y = train_data[r_number, dim]
    }
    prev_y = y

    object_features = train_data[r_number, -dim]
    object_features = (object_features - normalizer$Means) / normalizer$Vars
    object_features = as.numeric(c(-1, object_features))

    out = weights %*% object_features
    if (out * y <= 0) {
      # update weights
      old_weights = weights
      weights = weights + learning_rate * object_features * (y - out)
      print(abs(sum(weights - old_weights)))
    }
    train_errors[i] = getError(train_data, weights)
    test_errors[i] = getError(test_data, weights)

    if (test_errors[i] < min_error) {
      min_error = test_errors[i]
      good_weights = weights
    }
  }

  if (draw == T) {
    lab = sprintf("Min error: %f", min(test_errors))
    plot(test_errors, pch=21, col="white", ylim=c(0, 0.7), main=lab)
    lines(1:iterations_count, rep(0.2, iterations_count))
    lines(1:iterations_count, train_errors, col="green")
    lines(1:length(test_errors), test_errors, col="red")
    print(good_weights)
  }

  return (good_weights)
}

#plot(test_errors)

good_weights = calcWeights(train_data, test_data, weights)

Margin = calcMargin(train_data, good_weights)
#plot(1:length(Margin), sort(Margin), type='l', col="blue")
#lines(c(0, 350), c(0, 0))

indexes = sort(Margin, index.return=T)$ix

new_train_data = train_data[indexes[30:length(indexes)], ]

good_weights = calcWeights(new_train_data, test_data, good_weights, draw=T, rate=0.01, iters=500)

Margin = calcMargin(train_data, good_weights)
#plot(1:length(Margin), sort(Margin), type='l', col="blue")
#lines(c(0, 350), c(0, 0))
