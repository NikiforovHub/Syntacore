fid = fopen('output.txt','w'); 

formatSpec = '%i\t%i\n';
for i = 0:size(data,2)
    fprintf(fid, formatSpec, i, counter(i+1)) ;
end

fclose(fid) ;