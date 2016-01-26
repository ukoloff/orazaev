rm(list=ls())

library('date')



loadData = function() {
  games = read.csv('processed_games.csv')
  games$Date = as.Date(games$Date)

  return(games)
}



monthToGames = function(data, need.plot=TRUE) {
  dates = sort(data$Date)

  normalizeMonth = function(d) {
    temp.d = date.mdy(d)
    return(as.Date(sprintf('%d-%d-1', temp.d$year, temp.d$month)))
  }

  plotData = data.frame(Month=c(normalizeMonth(dates[1])), NumGames=c(1))

  for (i in c(2:length(dates))) {
    d = dates[i]
    last.date = plotData$Month[nrow(plotData)]

    if (months.Date(last.date) == months.Date(d)) {
      plotData$NumGames[nrow(plotData)] = plotData$NumGames[nrow(plotData)] + 1
    } else {
      plotData = rbind(plotData, data.frame(Month=c(normalizeMonth(d)), NumGames=c(1)))
    }
  }

  if (need.plot) {
    plot(plotData, xaxt='n', type='l', ylab='Число игр', xlab='', col='blue')

    axis.Date(1, at=plotData$Month, labels=format(plotData$Month, '%b %y'), las = 2)
  }

  return(plotData)
}



calcRedVictProb = function(game, raitings, sigma=800) {
  getRaiting = function(pid) {
    if (pid > 9000 || pid < 0) {
      return(700)
    }

    return(raitings[raitings$PID == pid, "Score"])
  }

  black.raiting = getRaiting(game$Maf1) + getRaiting(game$Maf2) + 2 * getRaiting(game$Don)
  black.raiting = black.raiting / 4


  red.raiting = 3 * getRaiting(game$Sheriff)

  for (i in c(1:10)) {
    pid = game[,sprintf("P%d", i)]

    if (pid != game$Don && pid != game$Maf1 && pid != game$Maf2 && pid != game$Sheriff) {
      red.raiting = red.raiting + getRaiting(pid)
    }
  }

  red.raiting = red.raiting / 9

  red.prob = 1 / (1 + 10^((black.raiting - red.raiting)/sigma))

  return(red.prob)
}


is.black = function(game, pid) {
  return(pid == game$Maf1 || pid == game$Maf2 || pid == game$Don)
}


calcEloReiting = function(data, newbieRaiting=700, mu=10, sigma=800) {

  raitings = data.frame()

  for (i in c(1:nrow(data))) {
  #for (i in c(1:2)) {
    cur.game = data[i,]

    # Get raitings
    for (p in c(1:10)) {
      pn = sprintf('P%d', p)

      pid = cur.game[1, pn]

      if (pid == -1) {
        next
      }

      # is it new player?
      if (! any(raitings$PID == pid)) {
        raitings = rbind(raitings, data.frame(PID=pid, Score=newbieRaiting))
      }
    }

    red.prob = calcRedVictProb(cur.game, raitings, sigma=sigma)
    print(sprintf("Prob: %f, Result: %d", red.prob, sum(cur.game$RedWins)))

    # Update players raiting
    for (i in c(1:10)) {
      pid = cur.game[,sprintf('P%d', i)]

      coef = 1.0
      if (pid == cur.game$Don || pid == cur.game$Sheriff) {
        coef = 1.5
      }

      if (is.black(cur.game, pid) == cur.game$RedWins) {
        coef = coef * -1
      }

      if (is.black(cur.game, pid)) {
        coef = coef*(1 - red.prob)
      } else {
        coef = coef*red.prob
      }

      raitings[raitings$PID==pid,"Score"] =
          raitings[raitings$PID==pid,"Score"] + coef*mu
    }
  }

  return(raitings)
}


games = loadData()


# Calculate num_games per month.
#plotData = monthToGames(games)

elo = calcEloReiting(games)
orderedElo = elo[order(elo$Score, decreasing = T),]
