%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function projections=buildProjectionsFromSinogram(aSinogram)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    width = size(aSinogram, 1);
    slice_number = size(aSinogram, 3);
    angle_number = size(aSinogram, 2);

    % Allocate the memory
    projections = zeros([width slice_number angle_number]);

    % Process all the angle
    for angle_id = 1:angle_number

        % Process all the slices
        for slice_id = 1:slice_number
            % Copy the data
            projections(:,slice_id,angle_id) = aSinogram(:,angle_id,slice_id);
        endfor
    endfor

    % Return the projections
    return;

endfunction
