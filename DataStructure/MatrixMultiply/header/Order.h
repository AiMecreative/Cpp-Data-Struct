#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace Order {
    enum struct Sequence {
        I,
        J,
        K
    };

    std::vector<Sequence> getOrder(const std::string &order) {
        if (order == "ijk") {
            return {Sequence::I, Sequence::J, Sequence::K};
        }
        if (order == "ikj") {
            return {Sequence::I, Sequence::K, Sequence::J};
        }
        if (order == "jik") {
            return {Sequence::J, Sequence::I, Sequence::K};
        }
        if (order == "jki") {
            return {Sequence::J, Sequence::K, Sequence::I};
        }
        if (order == "kij") {
            return {Sequence::K, Sequence::I, Sequence::J};
        }
        if (order == "kji") {
            return {Sequence::K, Sequence::J, Sequence::I};
        }
        return {};
    }

    std::ostream &operator<<(std::ostream &out, Sequence seq) {
        switch (seq) {
            case Sequence::I:
                out << "i" << " ";
                break;
            case Sequence::J:
                out << "j" << " ";
                break;
            case Sequence::K:
                out << "k" << " ";
                break;
            default:
                out << " ";
        }
        return out;
    }
}