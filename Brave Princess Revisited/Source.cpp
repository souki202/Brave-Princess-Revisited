#include <iostream>
#include <utility>
#include <queue>
#include <stack>
#include <algorithm>
#include <cstring>
#include <functional>

std::pair<int, int> road[101][101]; //first距離 second敵
int dp[101][101];//[残金][場所] = 既に襲われた数

struct Node {
	int pos, money, attacked;
	bool operator > (const Node& rhs) const {
		return attacked > rhs.attacked;
	}
	bool operator < (const Node& rhs) const {
		return attacked < rhs.attacked;
	}
};

int main() {
	int n, m, l;
	while (std::cin >> n >> m >> l && (n || m || l)) {
		std::memset(dp, -1, sizeof(dp));
		for (int i = 0; i < 101; i++) for (int j = 0; j < 101; j++) road[i][j] = std::make_pair(0, 0);
		for (int i = 0; i < m; i++) {
			int x, y, d, e;
			std::cin >> x >> y >> d >> e;
			road[x][y] = std::make_pair(d, e);
			road[y][x] = std::make_pair(d, e);
		}

		std::stack<Node> s;
		//std::priority_queue<Node> s;
		s.push({1, l, 0});

		int min = 1919810;
		while (!s.empty()) {
			auto node = s.top(); s.pop();

			for (int i = 1; i <= n; i++) {
				if (!road[node.pos][i].first) continue;
				auto next = node;
				next.pos = i;

				//雇わない
				{
					auto n2 = next;
					n2.attacked += road[node.pos][i].second;
					if (dp[n2.money][i] == -1 || dp[n2.money][i] > n2.attacked) {
						dp[n2.money][i] = n2.attacked;
						if (i == n) min = std::min(min, n2.attacked); //ごーーる
						else {
							//よりお金を残して今と同等かそれ以下の襲撃数なら意味なし
							bool isOk = true;
							for (int j = n2.money + 1; j <= l; j++) {
								if (dp[j][i] >= 0 && dp[j][i] <= n2.attacked) {
									isOk = false;
									break;
								}
							}
							if (isOk) {
								s.push(n2);
							}
						}
					}
				}
				//雇う
				if (next.money >= road[node.pos][i].first) {
					next.money -= road[node.pos][i].first;
					if (dp[next.money][i] == -1 || dp[next.money][i] > next.attacked) {
						dp[next.money][i] = next.attacked;
						if (i == n) min = std::min(min, next.attacked); //ごーーる
						else {
							//よりお金を残して今と同等かそれ以下の襲撃数なら意味なし
							bool isOk = true;
							for (int j = next.money + 1; j <= l; j++) {
								if (dp[j][i] >= 0 && dp[j][i] <= next.attacked) {
									isOk = false;
									break;
								}
							}
							if (isOk) {
								s.push(next);
							}
						}
					}
				}
			}
		}
		std::cout << min << std::endl;
	}

	return 0;
}