%% Spline Interpolation of Sine Data
% This generates a sine curve, then samples the spline over a finer mesh.

% Copyright 2015 The MathWorks, Inc.
clearvars
clear all
y=randi([-10 10],1,1e+4);
x=(0:length(y)-1);
xx=randi([0 1e+4],1,200);
xx=sort(xx);
ii=[];
for i=1:length(xx)
a=find(xx(i)==x);
if ~isempty(a)
    ii=[ii a];
end
end  

r=y;
r(ii)=[];
xd=x;
xd(ii)=[];



% %y(xx)=100;
%load('linear');
yy = interp1(xd,r,xx,'linear');

yn = interp1(xd,r,xx,'nearest');

yc = interp1(xd,r,xx,'v5cubic','extrap');


%figure();plot(x,r,'r',xx,yy,'b')
fid=fopen('interp_tests.cpp','w');
fprintf(fid,'#include <iostream>\n#include <vector>\n using namespace std;\n\n');
fprintf(fid,'vector<double> values={\n');
for i=1:length(y)-1
fprintf(fid,'%f,',y(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',y(end));
fprintf(fid,'};\n');


fprintf(fid,'vector<double> xd_interp={\n');
for i=1:length(x)-1
fprintf(fid,'%f,',x(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',x(end));
fprintf(fid,'};\n');

fprintf(fid,'vector<double> xi_interp={\n');
for i=1:length(xx)-1
fprintf(fid,'%f,',xx(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',xx(end));
fprintf(fid,'};\n');


fprintf(fid,'vector<double> expected_linear_interp={\n');
for i=1:length(yy)-1
fprintf(fid,'%f,',yy(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%ff\n',yy(end));
fprintf(fid,'};\n');
fprintf(fid,'vector<double> expected_nearest_interp={\n');
for i=1:length(yn)-1
fprintf(fid,'%f,',yn(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',yn(end));
fprintf(fid,'};\n');


fprintf(fid,'vector<double> expected_cubic_interp={\n');
for i=1:length(yc)-1
fprintf(fid,'%f,',yc(i));
if rem(i,20)==0
 fprintf(fid,'\n');
end
end
fprintf(fid,'%f\n',yc(end));
fprintf(fid,'};\n');

%save('linear','r','x','xx','yy');
fclose(fid);
