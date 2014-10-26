%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function volume=loadVolume(aFileNameWithExtension,
                           aDataType,
                           aWidth,
                           aHeight,
                           aDepth)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % Allocate the memory
    volume = zeros([aWidth aHeight aDepth]);
    slice = zeros([aWidth aHeight]);

    % Open the file
    file_descriptor = fopen(aFileNameWithExtension, "rb");

    % Read all the slices
    for slice_id = 1:aDepth

        % Read the sinogram
        slice = fread(file_descriptor, [aWidth, aHeight], aDataType);

      % Get the corresponding slice
      volume(:, :, slice_id) = slice;
    endfor

    % Close the file
    fclose(file_descriptor);

    % Return the volume
    return;

endfunction

