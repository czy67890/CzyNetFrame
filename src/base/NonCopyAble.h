#pragma once
class NonCopyAble{
public:
    NonCopyAble() = default;
    NonCopyAble(const NonCopyAble &rhs) = delete;
    NonCopyAble& operator=(const NonCopyAble &ths) = delete;
    NonCopyAble (NonCopyAble &&rhs) = default;
    NonCopyAble& operator=(NonCopyAble && rhs) = default; 
};