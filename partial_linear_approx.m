function out = partial_linear_approx(data_in,...
                                    coefs, dots)
    out = [];
    in = find(data_in > dots(1,2));
    out = [out; ones(size(in)).*dots(1,1)];
    for i = 1:length(coefs)
        in = data_in(find((data_in <= dots(i,2)) &...
                  (data_in > dots(i+1,2))));
        k = coefs(i, 1);
        b = coefs(i, 2);
        y = in .* k + b;
        out = [out; y];
    end
    in = find(data_in < dots(end,2));
    if ~isempty(in)
        out = [out; ones(size(in)).*dots(end,1)];
    end
end


% figure(3);
% plot(data_in, out);
% grid on;grid minor;
% 
% i=1;
% i=i+1;