%reading data
fid = fopen('input.txt');
tline = fgetl(fid);
data = split(tline, '');
data = str2num(cell2mat(data));
data = data';

i = 2;
tline = fgetl(fid);
while ischar(tline)
    data_tmp = split(tline, '');
    data_tmp = str2num(cell2mat(data_tmp));
    data_tmp = data_tmp';
    
    data = [data; data_tmp];
    
    tline = fgetl(fid);
    i = i + 1;
end
fclose(fid);