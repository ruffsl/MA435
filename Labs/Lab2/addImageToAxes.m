function [ imageObject ] = addImageToAxes( imageFileName, axesHandle, axesWidth )
%ADDIMAGETOAXES Add an image to an axes
%   Opens the image filename and adds it to the axes
%   Return the image object
%   If axesWidth = 0 then use images default pixel size

%Load file to get image data
imageData = imread(imageFileName);
%Create an image object and make the parent the axes
imageObject = image(imageData, 'Parent', axesHandle);
%Make units of the axes pixels
set(axesHandle, 'Units', 'Pixels');
%Visible off
set(axesHandle, 'Visible', 'Off');
%Get the current 'Position' of the Axes so that we can use the x and y
currentPosition = get(axesHandle, 'Position');
% Get the number of rows and columns of the image
[rows_height, cols_width, depth] = size(imageData);
%If axes width = 0
if axesWidth == 0
    axesWidth = cols_width;
    axesHeight = rows_height;
else
%else
    % Use the axesWidth and aspect ratio to calculate the height
    axesHeight = axesWidth * rows_height/cols_width;
end
%end

%set the new 'Position' on the axes
set(axesHandle, 'Position', [currentPosition(1), currentPosition(2), axesWidth, axesHeight]);
end

