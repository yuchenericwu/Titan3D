function M = parse_matrix(filename, x, y, z)
    M = csvread(filename);
    M = reshape(M(1:end-1), x, y, z);
    
    % Reorient to column-major
    for i = 1:z
        M(1:x,1:y,i) = M(1:x,1:y,i)';
    end
end