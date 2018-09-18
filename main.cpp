#include "olcPixelGameEngine.h"

#include <vector>
#include <random>
#include <ctime>

struct Pixel {
	int x, y;
};

struct Node {
	float x, y;
	float velX, velY;
	Node* connection;
};

struct Character {
	int offset;
	std::vector<Pixel> pixels;
	int index = 0;
};

class App : public olc::PixelGameEngine {

private:
	float m_time;
	float m_ready;
	const int pixelSize = 4;
	int totalPixels;
	int brightness;
	int xOffset, yOffset;
	float interval;
	const std::string m_text = "Test String";
	std::vector<Character> m_characters;
	std::vector<Node> m_nodes;

public:
	bool OnUserCreate() override {
		sAppName = "Pixel Test";
		srand(time(0));

		for (int i = 0; i < 80; i++)
			m_nodes.push_back({ float(rand() % ScreenWidth()), float(rand() % ScreenHeight()), float(rand() % 10 - 5) / 30.0f, float(rand() % 10 - 5) / 30.0f });

		Character R{ 6 };
		for (int i = 7; i >= 0; i--)
			R.pixels.push_back({ 0, i });
		for (int i = 1; i <= 3; i++)
			R.pixels.push_back({ i, 0 });
		for (int i = 1; i <= 2; i++)
			R.pixels.push_back({ 4, i });
		for (int i = 3; i >= 1; i--)
			R.pixels.push_back({ i, 3 });
		for (int i = 4; i <= 7; i++)
			R.pixels.push_back({ 4, i });

		Character e{ 6 };
		for (int i = 4; i >= 1; i--)
			e.pixels.push_back({ i, 7 });
		for (int i = 6; i >= 4; i--)
			e.pixels.push_back({ 0, i });
		for (int i = 1; i <= 3; i++)
			e.pixels.push_back({ i, 3 });
		for (int i = 4; i <= 5; i++)
			e.pixels.push_back({ 4, i });
		for (int i = 3; i >= 1; i--)
			e.pixels.push_back({ i, 5 });

		Character t{ 5 };
		for (int i = 3; i >= 2; i--)
			t.pixels.push_back({ i, 7 });
		for (int i = 6; i >= 0; i--)
			t.pixels.push_back({ 1, i });
		t.pixels.push_back({ 0, 2 });
		t.pixels.push_back({ 2, 2 });

		Character u{ 6 };
		for (int i = 3; i <= 6; i++)
			u.pixels.push_back({ 0, i });
		for (int i = 1; i <= 3; i++)
			u.pixels.push_back({ i, 7 });
		for (int i = 6; i >= 3; i--)
			u.pixels.push_back({ 4, i });

		Character r{ 5 };
		for (int i = 7; i >= 4; i--)
			r.pixels.push_back({ 0, i });
		for (int i = 1; i <= 3; i++)
			r.pixels.push_back({ i, 3 });

		Character n{ 6 };
		for (int i = 7; i >= 3; i--)
			n.pixels.push_back({ 0, i });
		for (int i = 1; i <= 3; i++)
			n.pixels.push_back({ i, 3 });
		for (int i = 4; i <= 7; i++)
			n.pixels.push_back({ 4, i });

		Character N{ 6 };
		for (int i = 7; i >= 0; i--)
			N.pixels.push_back({ 0, i });
		for (int i = 1; i <= 3; i++)
			N.pixels.push_back({ i, i });
		for (int i = 0; i <= 7; i++)
			N.pixels.push_back({ 4, i });

		Character l{ 3 };
		for (int i = 7; i >= 0; i--)
			l.pixels.push_back({ 0, i });

		Character space{ 6 };

		m_characters.push_back(R);
		m_characters.push_back(e);
		m_characters.push_back(t);
		m_characters.push_back(u);
		m_characters.push_back(r);
		m_characters.push_back(n);
		m_characters.push_back(space);
		m_characters.push_back(N);
		m_characters.push_back(u);
		m_characters.push_back(l);
		m_characters.push_back(l);

		int totalOffset = 0;
		totalPixels = 0;
		for (Character& chr : m_characters) {
			totalPixels += chr.pixels.size();
			totalOffset += chr.offset;
		}

		interval = 5.0f / totalPixels;

		brightness = 0;

		yOffset = ScreenHeight() / 2 - 3.5 * pixelSize;
		xOffset = ScreenWidth() / 2 - totalOffset * pixelSize / 2;

		SetPixelMode(olc::Pixel::Mode::ALPHA);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(brightness, brightness, brightness));

		m_time += fElapsedTime;
		if (m_ready <= 1.0f)
			m_ready += fElapsedTime;
		if (m_time > interval) {
			m_time -= interval;

			if (m_ready > 1.0f) {
				// Increments the pixel index of the characters
				for (Character& chr : m_characters) {
					if (chr.index >= chr.pixels.size())
						continue;
					else {
						chr.index++;
						break;
					}
				}

				// Increase the brightness if all the pixels are drawn
				if (m_characters.back().index >= m_characters.back().pixels.size()) {
					brightness += 8;

					// Check if brightness is below the maximum
					brightness = brightness > 255 ? 255 : brightness;
				}
			}

			for (Node& p1 : m_nodes) {
				// Checks if the node is off the screen and resets it to the other side
				p1.x = p1.x < 0 ? ScreenWidth() : p1.x > ScreenWidth() ? 0 : p1.x;
				p1.y = p1.y < 0 ? ScreenHeight() : p1.y > ScreenHeight() ? 0 : p1.y;
			}

			for (Node& p1 : m_nodes) {
				bool connected = false;
				for (Node& p2 : m_nodes) {

					// Checks if the current node is itself and continues if it is
					if (&p1 == &p2) continue;
					float dx = (p1.x - p2.x);
					float dy = (p1.y - p2.y);

					// Checks if the distance between the two nodes is less than 40
					if (dx * dx + dy * dy < 40 * 40) {
						connected = true;
						p1.connection = &p2;
					}
				}

				// Removes the existing connection if no connections with other nodes were made
				if (!connected)
					p1.connection = nullptr;
			}
		}

		for (Node& p : m_nodes) {
			// Adds the nodes velocity to it's position
			p.x += p.velX;
			p.y += p.velY;

			// Draws the node
			DrawRect(p.x - 1, p.y - 1, 3, 3, olc::Pixel(80, 180, 255, 64 - (brightness + 1)/ 4));

			// Draws the node's connection if it exists
			if (p.connection)
				DrawLine(p.x, p.y, p.connection->x, p.connection->y, olc::Pixel(80, 180, 255, 64 - (brightness + 1) / 4));
		}

		int offset = 0;
		for (Character& chr : m_characters) {
			for (int i = 0; i < chr.pixels.size(); i++)
				if (chr.index > i)
					FillRect((chr.pixels[i].x + offset) * pixelSize + xOffset, chr.pixels[i].y * pixelSize + yOffset, pixelSize, pixelSize);
			offset += chr.offset;
		}

		return true;
	}
};

int main() {
	App app;
	if (app.Construct(640, 360, 2, 2))
		app.Start();

	return 0;
}