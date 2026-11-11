classdef Node
    properties
        if_leave; % 0 是负 1 是正 2 是非叶
        Attribute_edge = [];%多个属性边
        Attribute_edge_index = [];% 多个属性变连接到的点
    end
end