function normalizedImg = imnormalize(img)

    if length(size(img)) 
        normalizedImg = rgb2gray(img);
    else
        normalizedImg = img;
    end
end
