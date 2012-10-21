function [Imgs] = loadImgsFromFile(file, RESIZE = 0)


    Imgs = cell();

    f = fopen(file, 'r');
    c = fscanf(f, "%c");
    
    imFiles = strsplit(c, "\n");



    for i = 1:length(imFiles)
        curFile = imFiles{i};
        printf("Loading %s...\n", curFile)

        if ~isempty(curFile)
            if RESIZE
                Imgs{i} = imresize(imread(curFile), [640 480]);
            else
                Imgs{i} = imread(curFile);
            end
        end
    end
end
