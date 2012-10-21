function score = calculateMatchScore(Match, Scores)

    score = mean(Scores) * length(Match);
end
