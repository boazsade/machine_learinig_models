%r=randi([-100 100],1,1e+4);
load('r');
%mapminmax  [Y,PS] = mapminmax(X,YMIN,YMAX);
%1. y = (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin;
%   [y,Ps] = mapminmax(r,YMIN,YMAX); YMIN=-1;YMAX=1;

[y,Ps] = mapminmax(r,-10,10);
[z,Ps2] = mapminmax(r,0,1);

y=y';
fid=fopen('minmax.cpp','w');
fprintf(fid,'#include <iostream>\n#include <vector>\n using namespace std;\n\n');
fprintf(fid,'vector<double> data={\n');
for i=1:length(r)-1
fprintf(fid,'%f,',r(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',r(end));
fprintf(fid,'};\n');


fprintf(fid,'vector<double> expected_first={\n');
for i=1:length(y)-1
fprintf(fid,'%f,',y(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%ff\n',y(end));
fprintf(fid,'};\n');
fprintf(fid,'vector<double> PsX={%f,%f};// normalization to (-10,10) \n',Ps.xmin,Ps.xmax);%ouput XMIN;XMAX
fprintf(fid,'vector<double> PsY={%f,%f};//normalization to (-10,10)\n',Ps.ymin,Ps.ymax);%input  YMIN;YMAX;  

fprintf(fid,'vector<double> expected_second={\n');
for i=1:length(z)-1
fprintf(fid,'%f,',z(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end

end
fprintf(fid,'%f\n',z(end));
fprintf(fid,'};\n');
fprintf(fid,'vector<double> PsX={%f,%f};// normalization to (0,1) \n',Ps2.xmin,Ps2.xmax);%ouput XMIN;XMAX
fprintf(fid,'vector<double> PsY={%f,%f};//normalization to (0,1)\n',Ps2.ymin,Ps2.ymax);%input  YMIN;YMAX;  

fclose(fid);
figure();plot(y)
%save('r','r');