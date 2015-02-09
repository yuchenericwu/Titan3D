function M = parse_matrix(filename, x, y, z)
    M = csvread(filename);
    M = reshape(M(1:end-1), x, y, z);
end