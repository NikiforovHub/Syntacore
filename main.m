readMy;

counter = zeros(1, size(data,2) + 1);

for k = 0:(2^(size(data,1)) - 1)
    binStr = dec2bin(k,size(data,1));
    binStr = split(binStr, '');
    binStr = str2num(cell2mat(binStr));
    binStr = binStr';
    
    vec = binStr*data;
    vec = rem(vec, 2);
    
    w = weight(vec);
    
    counter(w + 1) = counter(w + 1) + 1;
end


bar(0:size(data,2), counter);

writeMy;




