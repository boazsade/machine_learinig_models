%1. x/max(abs(x));
clearvars
close all
r = 24*normrnd(0.7,0.123,1,1e+4);

I = randi([100 1e+4],1,1e+2);
%I=I(find((I> 100)));
r(I)=I;

load('out_n');

fid=fopen('outlier_data.cpp','w');
fprintf(fid,'#include <iostream>\n#include <vector>\n using namespace std;\n\n');
fprintf(fid,'vector<double> data_outlier={\n');
for i=1:length(r)-1
fprintf(fid,'%f,',r(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',r(end));
fprintf(fid,'};\n');




I=sort(I);
fprintf(fid,'vector<size_t> expected_outlier={\n');
for i=1:length(I)-1
fprintf(fid,'%i,',I(i)-1);
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%i\n',I(end)-1);
fprintf(fid,'};\n');



fclose(fid);
figure();plot(I);
figure();plot(r);
%save('out_n','r');
