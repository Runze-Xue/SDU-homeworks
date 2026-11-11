Data = xlsread('ex6Data.csv');
c = cvpartition(length(Data(:, 1)),'KFold',10);
for i = 1 : c.NumTestSets
    test_data = Data(test(c, i));
    train_data = Data(training(c, i));
end