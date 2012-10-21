function [Matched, Score, Perm] = imGroupMatch(Img, CellOfImates, THRESH = 2.0)

    Matched = cell();
    Score = zeros(length(CellOfImates), 1);

    printf("Analysing input img ...");
    [F, D] = imsift(imnormalize(Img));
    printf(" OK \n");

    for i = 1:length(CellOfImates)
        printf("Match image %d ...", i);
        candidate = CellOfImates{i};
        [curF, curD] = ...
            imsift(imnormalize(candidate));

        [curMatched, curScore] = ...
            vl_ubcmatch(D, curD, THRESH);

        Matched{i} = curMatched;
        Score(i) = ...
            calculateMatchScore(curMatched, curScore);
        printf("score %d\n", Score(i));
    end

    [sorted, Perm] = sort(Score);

end
