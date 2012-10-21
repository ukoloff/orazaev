function [f, d] = imsift(img)
%
% imsift(image) -- find feature and descriptors.
%
% required: 
%   * installed vlfeat library.
%   * vlfeat functions in path.
%

    isRgbImage = ...
        length(size(img)) > 2;
    
    if isRgbImage
        gray = rgb2gray(img);
    else
        gray = img;
    end

    [f, d] = vl_sift(single(gray));
end
