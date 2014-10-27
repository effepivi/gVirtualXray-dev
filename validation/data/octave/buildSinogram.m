%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function sinogram=buildSinogram(aProcessNumber,
                                aCTVolume,
                                anAngleSpan)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    slice_id_set = 1:size(aCTVolume, 3);
    sinogram = cell2mat(pararrayfun(aProcessNumber, @(slice_id_set) runParallelRadonTransform(aCTVolume, anAngleSpan, slice_id_set), slice_id_set, "UniformOutput", false, "VerboseLevel", 2));


    sinogram = reshape(sinogram, size(sinogram, 1), (size(sinogram, 2) / size(aCTVolume, 3)), size(aCTVolume, 3));
 
    % Return the CT volume
    return;

endfunction

