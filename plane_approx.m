dots = {
    [69 5.14 5.15; 483 2.93 5.15]
    [28 4.51 4.51; 434 2.56 4.51]
    };
coefs = {};
for i=1:length(dots)
    dot = dots{i};
    xyz1 = dot(1, :);
    xyz2 = dot(2, :);
    
    k = (xyz2(2) - xyz1(2)) / (xyz2(1) - xyz1(1));
    b = xyz1(2) - k .* xyz1(1);
    
    k = k / xyz1(3);
    b = b / xyz1(3);
    
    coefs{i} = [k b xyz1(3)];
    disp(['k=', num2str(k), '  b=', num2str(b)]);
end

kb = {};

for i=1:(length(coefs{1})-1)
    
    dot1 = [coefs{1}(i) coefs{1}(end)];
    dot2 = [coefs{2}(i) coefs{2}(end)];
    
    k = (dot2(1) - dot1(1)) / (dot2(2) - dot1(2));
    b = dot1(1) - k .* dot1(2);
     
    kb{i} = [k b];
    disp(['kb k=', num2str(k), '  kb b=', num2str(b)]);
end