%1. x/max(abs(x));
clearvars
close all
r = 24*normrnd(0.7,0.123,1,1e+4);
load('z_n');
maxval=max(abs(r));

y= r/maxval;
fid=fopen('zmaxnorm.cpp','w');
fprintf(fid,'#include <iostream>\n#include <vector>\n using namespace std;\n\n');
fprintf(fid,'vector<double> data_zmax={\n');
for i=1:length(r)-1
fprintf(fid,'%f,',r(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',r(end));
fprintf(fid,'};\n');


fprintf(fid,'vector<double> expected_zmax={\n');
for i=1:length(y)-1
fprintf(fid,'%f,',y(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%ff\n',y(end));
fprintf(fid,'};\n');


fprintf(fid,'float Ps=%f;\n',maxval);

fclose(fid);
figure();plot(y);
figure();plot(r);
save('z_n','r');
