init N = 32;
clear K;
clear R;

while N not 0 do;
  copy N to K;
  while K not 0 do;
    incr R;
    decr K;
  end;
  decr N;
end;
