%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function writeVolume(aFileNameWithExtension,
                     aDataType,
                     aVolume)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	% Open the file
    file_descriptor = fopen(aFileNameWithExtension, "wb");

	% Write all the slices
    fwrite(file_descriptor, aVolume, aDataType);

    % Close the file
    fclose(file_descriptor);

    size(aVolume)
    
    return;

endfunction

