%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function ct_volume=reconstructVolume(aProcessNumber,
                                     aSinogram,
                                     anAngleSpan)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    slice_id_set = 1:size(aSinogram, 3);
    ct_volume = pararrayfun(aProcessNumber, @(slice_id_set) runParallelReconstruction(aSinogram, anAngleSpan, slice_id_set), slice_id_set, "UniformOutput", false, "VerboseLevel", 2);

    ct_volume = reshape(ct_volume, size(ct_volume, 1), size(ct_volume, 1), (size(ct_volume, 2) / size(ct_volume, 1)));
 
    % Return the CT volume
    return;

endfunction





