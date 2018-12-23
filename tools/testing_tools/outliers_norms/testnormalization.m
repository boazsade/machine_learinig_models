% k=5;
% r = -k + 2*k*rand(10000,1);
%figure();plot(r);
% save('Randomdata.mat','r');

load('Randomdata.mat');
figure();plot(r);








% fid=fopen('Random.h','w');
% fprintf(fid,'float r={\n');
% for i=1:length(r)-1
% fprintf(fid,'%ff,\n',r(i));
% end
% fprintf(fid,'%ff\n',r(end));
% fprintf(fid,'};\n');
% fclose(fid);


