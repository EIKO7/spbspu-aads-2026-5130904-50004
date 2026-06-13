#include "hashtable.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace ahrameev {

struct Graph
{
    std::string name;
    Vector<std::string> vertices;
    HashTable<
        std::pair<std::string, std::string>,
        Vector<uint32_t>,
        SipHash,
        PairEq
    > edges;

    Graph()
        : edges(16)
    {
    }
};

Vector<std::string> split(const std::string& str)
{
    Vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(' ');
    while (end != std::string::npos) {
        if (end > start) {
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + 1;
        end = str.find(' ', start);
    }
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}

void run(const std::string& filename)
{
    HashTable<std::string, Graph, SipHash, StrEq> graphs_db(32);
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string token;
    while (file >> token) {
        std::string gname = token;
        size_t edges_count = 0;
        if (!(file >> edges_count)) {
            break;
        }
        Graph g;
        g.name = gname;
        for (size_t i = 0; i < edges_count; ++i) {
            std::string v1;
            std::string v2;
            uint32_t w = 0;
            file >> v1 >> v2 >> w;
            if (!g.vertices.contains(v1)) {
                g.vertices.push_back(v1);
            }
            if (!g.vertices.contains(v2)) {
                g.vertices.push_back(v2);
            }
            std::pair<std::string, std::string> edge = {v1, v2};
            if (g.edges.has(edge)) {
                g.edges.get(edge).push_back(w);
            } else {
                Vector<uint32_t> wv;
                wv.push_back(w);
                g.edges.add(edge, wv);
            }
        }
        graphs_db.add(gname, std::move(g));
    }
    file.close();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        Vector<std::string> t = split(line);
        if (t.empty()) {
            continue;
        }
        const std::string& cmd = t[0];

        try {
            if (cmd == "graphs") {
                Vector<std::string> names;
                for (auto it = graphs_db.begin(); it != graphs_db.end(); ++it) {
                    names.push_back((*it).key);
                }
                names.sort();
                for (size_t i = 0; i < names.size(); ++i) {
                    std::cout << names[i] << "\n";
                }
            } else if (cmd == "vertexes") {
                if (t.size() != 2 || !graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Graph& g = graphs_db.get(t[1]);
                Vector<std::string> v = g.vertices;
                v.sort();
                for (size_t i = 0; i < v.size(); ++i) {
                    if (i == 0 || v[i] != v[i - 1]) {
                        std::cout << v[i] << "\n";
                    }
                }
            } else if (cmd == "outbound" || cmd == "inbound") {
                if (t.size() != 3 || !graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Graph& g = graphs_db.get(t[1]);
                if (!g.vertices.contains(t[2])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }

                struct EdgeInfo {
                    std::string node;
                    uint32_t w;

                    bool operator<(const EdgeInfo& o) const
                    {
                        if (node != o.node) {
                            return node < o.node;
                        }
                        return w < o.w;
                    }
                };

                Vector<EdgeInfo> edges_list;
                for (auto it = g.edges.begin(); it != g.edges.end(); ++it) {
                    bool match = (cmd == "outbound")
                        ? ((*it).key.first == t[2])
                        : ((*it).key.second == t[2]);
                    if (match) {
                        std::string target = (cmd == "outbound")
                            ? (*it).key.second
                            : (*it).key.first;
                        for (size_t i = 0; i < (*it).value.size(); ++i) {
                            edges_list.push_back({target, (*it).value[i]});
                        }
                    }
                }
                edges_list.sort();
                for (size_t i = 0; i < edges_list.size(); ) {
                    std::cout << edges_list[i].node;
                    size_t j = i;
                    while (j < edges_list.size() && edges_list[j].node == edges_list[i].node) {
                        std::cout << " " << edges_list[j].w;
                        ++j;
                    }
                    std::cout << "\n";
                    i = j;
                }
            } else if (cmd == "bind") {
                if (t.size() != 5 || !graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Graph& g = graphs_db.get(t[1]);
                if (!g.vertices.contains(t[2])) {
                    g.vertices.push_back(t[2]);
                }
                if (!g.vertices.contains(t[3])) {
                    g.vertices.push_back(t[3]);
                }
                std::pair<std::string, std::string> edge = {t[2], t[3]};
                uint32_t w = std::stoul(t[4]);
                if (g.edges.has(edge)) {
                    g.edges.get(edge).push_back(w);
                } else {
                    Vector<uint32_t> wv;
                    wv.push_back(w);
                    g.edges.add(edge, wv);
                }
            } else if (cmd == "cut") {
                if (t.size() != 5 || !graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Graph& g = graphs_db.get(t[1]);
                if (!g.vertices.contains(t[2]) || !g.vertices.contains(t[3])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                std::pair<std::string, std::string> edge = {t[2], t[3]};
                if (!g.edges.has(edge)) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Vector<uint32_t>& wv = g.edges.get(edge);
                if (!wv.erase_value(std::stoul(t[4]))) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                if (wv.empty()) {
                    g.edges.drop(edge);
                }
            } else if (cmd == "create") {
                if (t.size() < 3 || graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                size_t k = std::stoul(t[2]);
                if (t.size() != 3 + k) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Graph ng;
                ng.name = t[1];
                for (size_t i = 0; i < k; ++i) {
                    ng.vertices.push_back(t[3 + i]);
                }
                graphs_db.add(t[1], std::move(ng));
            } else if (cmd == "merge") {
                if (t.size() != 4 || !graphs_db.has(t[2]) ||
                    !graphs_db.has(t[3]) || graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                const Graph& g1 = graphs_db.get(t[2]);
                const Graph& g2 = graphs_db.get(t[3]);
                Graph ng;
                ng.name = t[1];
                for (size_t i = 0; i < g1.vertices.size(); ++i) {
                    if (!ng.vertices.contains(g1.vertices[i])) {
                        ng.vertices.push_back(g1.vertices[i]);
                    }
                }
                for (size_t i = 0; i < g2.vertices.size(); ++i) {
                    if (!ng.vertices.contains(g2.vertices[i])) {
                        ng.vertices.push_back(g2.vertices[i]);
                    }
                }

                auto copy_edges = [&](const Graph& src) {
                    for (auto it = src.edges.begin(); it != src.edges.end(); ++it) {
                        if (ng.edges.has((*it).key)) {
                            Vector<uint32_t>& tw = ng.edges.get((*it).key);
                            for (size_t i = 0; i < (*it).value.size(); ++i) {
                                tw.push_back((*it).value[i]);
                            }
                        } else {
                            ng.edges.add((*it).key, (*it).value);
                        }
                    }
                };

                copy_edges(g1);
                copy_edges(g2);
                graphs_db.add(t[1], std::move(ng));
            } else if (cmd == "extract") {
                if (t.size() < 4 || !graphs_db.has(t[2]) || graphs_db.has(t[1])) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                size_t k = std::stoul(t[3]);
                if (t.size() != 4 + k) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                const Graph& og = graphs_db.get(t[2]);
                Vector<std::string> ev;
                bool valid = true;
                for (size_t i = 0; i < k; ++i) {
                    if (!og.vertices.contains(t[4 + i])) {
                        valid = false;
                        break;
                    }
                    if (!ev.contains(t[4 + i])) {
                        ev.push_back(t[4 + i]);
                    }
                }
                if (!valid) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                Graph ng;
                ng.name = t[1];
                ng.vertices = ev;
                for (auto it = og.edges.begin(); it != og.edges.end(); ++it) {
                    if (ev.contains((*it).key.first) && ev.contains((*it).key.second)) {
                        ng.edges.add((*it).key, (*it).value);
                    }
                }
                graphs_db.add(t[1], std::move(ng));
            } else {
                std::cout << "<INVALID COMMAND>\n";
            }
        } catch (const std::exception& e) {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}

}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    try {
        ahrameev::run(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}