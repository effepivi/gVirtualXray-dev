%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ct_slice]=runParallelReconstruction(aSinogram,
                                              anAngleSpan,
                                              aSliceID)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % Reconstruct the slice
    ct_slice = iradon(aSinogram(:,:, aSliceID), anAngleSpan, "Spline", "Hamming");

    % Copy the data
    ct_slice = ct_slice(2:size(ct_slice, 1) - 1,2:size(ct_slice, 2) - 1);

    % Return the CT slice
    return;

endfunction





