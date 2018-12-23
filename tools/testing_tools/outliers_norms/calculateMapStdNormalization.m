%mapstd  [Y,PS] = mapstd(X,ymean,ystd)
%1. mapstd (x-mean(x))/std(x);
%   y=mapstd(x);
r=randi([-120 100],1,1e+4);
[y,Ps]=mapstd(r);
y=y';
fid=fopen('mapstd.cpp','w');
fprintf(fid,'#include <iostream>\n#include <vector>\n using namespace std;\n\n');
fprintf(fid,'vector<double> data_mapstd={\n');
for i=1:length(r)-1
fprintf(fid,'%f,',r(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',r(end));
fprintf(fid,'};\n');


fprintf(fid,'vector<double> expected_first_mapstd={\n');
for i=1:length(y)-1
fprintf(fid,'%f,',y(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%ff\n',y(end));
fprintf(fid,'};\n');
fprintf(fid,'vector<double> PsX={%f,%f};\n',Ps.xmean,Ps.xstd);%ouput XMIN;XMAX




fclose(fid);
figure();plot(y)