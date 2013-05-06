data = read.csv('train.csv')


replace.na.with.mean = function(data) {
  data.without.na = data.frame()
  classes = unique(data$y)

  for (c in classes) {
    cur.class.data = data[data$y == c,]

    for (field in names(cur.class.data)) {
      cur.field.na = is.na(cur.class.data[,field])
      cur.class.data[cur.field.na, field] = mean(cur.class.data[,field], na.rm=T)
    }

    data.without.na = rbind(data.without.na, cur.class.data)
  }

  return(data.without.na)
}

get.class.sizes = function(data) {
  class.sizes = data.frame()
  classes = unique(data$y)

  for (c in classes) {
    class.sizes = rbind(class.sizes, data.frame(size=nrow(data[data$y == c,]), y=c))
  }

  return(class.sizes)
}

oversampling = function(data) {
  balanced.data = data.frame()

  class.sizes = get.class.sizes(data)
  class.sizes = class.sizes[order(class.sizes$size, decreasing=T),]

  max.size = class.sizes$size[1]
  balanced.data = rbind(balanced.data, data[data$y == class.sizes$y[1],])

  for (i in 2:nrow(class.sizes)) {
    cur.y = class.sizes$y[i]
    cur.size = class.sizes$size[i]

    cur.class.data = data[data$y == class.sizes$y[i], -ncol(data)]

    while (cur.size < max.size) {
      add.size = round(cur.size * 0.3)
      if (cur.size + add.size > max.size) {
        add.size = max.size - cur.size
      }

      print(sprintf("Run kmeans on %d elements, with %d clusters.", cur.size, add.size))
      centroids = as.data.frame(kmeans(cur.class.data, add.size, 4000, 5)$centers)

      cur.class.data = rbind(cur.class.data, centroids)
      cur.size = nrow(cur.class.data)
      print(sprintf("New size = %d.", cur.size))
    }
    cur.class.data$y = rep(cur.y, nrow(cur.class.data))

    balanced.data = rbind(balanced.data, cur.class.data)
  }

  return(balanced.data)
}

undersampling = function(data) {
  balanced.data = data.frame()

  class.sizes = get.class.sizes(data)
  class.sizes = class.sizes[order(class.sizes$size, decreasing=F),]

  min.size = class.sizes$size[1]
  balanced.data = rbind(balanced.data, data[data$y == class.sizes$y[1],])

  for (i in 2:nrow(class.sizes)) {
    cur.y = class.sizes$y[i]
    cur.size = class.sizes$size[i]

    cur.class.data = data[data$y == cur.y,]

    while(cur.size > min.size) {
      rm.size = round(cur.size * 0.1)
      if (cur.size - rm.size < min.size) {
        rm.size = cur.size - min.size
      }

      print(sprintf("Run kmeans on %d elements, with %d clusters.", cur.size, rm.size))
      kmeans.result = kmeans(cur.class.data, rm.size, 4000, 5)
      print(sprintf("Removing centroids nearest point for each cluster.", cur.size))

      # Remove one point for each cluster
      filtered.data = data.frame()
      for (cluster in 1:rm.size ) {
        cur.cluster.data = cur.class.data[kmeans.result$cluster == cluster,]

        center = kmeans.result$center[cluster]

        # Calc nearest to centroid point index
        nearest.index = NA
        nearest.distance = Inf
        for (j in 1:nrow(cur.cluster.data)) {
          cur.distance = sqrt(sum((cur.cluster.data - center)^2))

          if (cur.distance < nearest.distance) {
            nearest.distance = cur.distance
            nearest.index = i
          }
        }

        # Remove nearest point
        cur.cluster.data = cur.cluster.data[-nearest.index,]
        filtered.data = rbind(filtered.data, cur.cluster.data)
      }

      cur.class.data = filtered.data
      cur.size = nrow(cur.class.data)
    }

    balanced.data = rbind(balanced.data, cur.class.data)
  }

  return (balanced.data)
}

calc.balanced.data = function(data, factor=0.5) {
  class.sizes = get.class.sizes(data)
  class.sizes = class.sizes[order(class.sizes$size, decreasing=T),]

  bigest.class = class.sizes$y[1]
  pseudo.max.size = round(class.sizes$size[1] * factor)

  if (pseudo.max.size < class.sizes$size[2]) {
    pseudo.max.size = class.size$size[2]
  }

  hiden.indexes = c(1:nrow(data))[data$y == bigest.class]
  hiden.indexes = hiden.indexes[1:(class.sizes$size[1] - pseudo.max.size)]

  pseudo.data = data[-hiden.indexes,]
  pseudo.data = oversampling(pseudo.data)
  pseudo.data = rbind(pseudo.data, data[hiden.indexes,])
  pseudo.data = undersampling(pseudo.data)

  return(pseudo.data)
}


calc.svm.model = function(data) {
  library('e1071')

  # TUNE = tune(svm, train.x=balanced.data[, -11], train.y=balanced.data[, 11], ranges=list(gamma=c(0.001, 0.003, 0.01, 0.03, 0.1, 0.3, 1, 3, 10, 30), cost=c(0.001, 0.003, 0.01, 0.03, 0.1, 0.3, 1, 3, 10, 30)), tunecontrol=tune.control(sampling="fix"))
  # TUNE$best.parameters = gamma=1, cost=3
  SVM = svm(data[, -ncol(data)], data[, ncol(data)], type='C', gamma=1, cost=3)

  return(SVM)
}

split.dataset = function(data, coefficient=0.7) {
  classes = unique(data$y)
  indexes = c()

  for (c in classes) {
    cur.data.indexes = c(1:nrow(data))[data$y == c]
    indexes = c(indexes, sample(cur.data.indexes, round(length(cur.data.indexes)*coefficient)))
  }

  return(indexes)
}


test.svm = function(data, nrepeat=5, gamma=0.97, cost=50, kernel='radial') {
  set.seed(13)
  accuracy = 0

  for (i in 1:nrepeat) {
    train.indexes = split.dataset(data)
    model = svm(data[train.indexes,-11], data[train.indexes,11], type='C', gamma=gamma, cost=cost,kernel=kernel)
    pred = predict(model, data[-train.indexes,-11])

    accuracy = accuracy + sum(pred == data[-train.indexes,11]) / nrow(data[-train.indexes,])

    print(sprintf("[%d] accuracy = %f", i, accuracy / i))
  }

  return(accuracy / nrepeat)
}


set.seed(42)

data = replace.na.with.mean(data)
#balanced.data = oversampling(data)
#SVM = calc.svm.model(balanced.data)

balanced.data = read.csv('train.balanced.csv')
