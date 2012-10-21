function [match, sources] = immatch(img0, img1, THRESH = 2.0)
%
% immatch(image0, image1) -- calculate vl_ubcmatch for images.
%
% required: 
%   * installed vlfeat library.
%   * vlfeat functions in path.
%

    [f0, d0] = imsift(img0);
    [f1, d1] = imsift(img1);

    [match, sources] = vl_ubcmatch(d0, d1, THRESH);

%    image(img0);
%    vl_plotframe(f0(:, match(1, :)));
%    
%    figure();
%    image(img1);
%    vl_plotframe(f1(:, match(2, :)));
end
