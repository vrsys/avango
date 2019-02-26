#ifndef FINGERINFO_H
#define FINGERINFO_H

#include <deque>
#include <set>

class FingerClient;

class Vec2
{
  public:
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}
    bool operator!=(const Vec2& v) { return (v.x != x || v.y != y); }
};

class FingerInfo
{
  public:
    FingerInfo(int id, Vec2 position);

    int getFingerId();
    int getUserId();
    void setUserId(const int id);

    void addPosition(Vec2 position);
    Vec2 getPosition(int index = 0);
    Vec2 getFilteredPosition();

    float getAngle() const;
    void setAngle(const float angle);

    float getWidth() const;
    float getHeight() const;
    float getArea() const;
    void setWidthHeightArea(const float width, const float height, const float area);

    void setPositionHistorySize(unsigned int size);

  private:
    typedef std::deque<Vec2> PositionHistory;

    int m_iFingerId, m_iUserId;
    float m_angle, m_width, m_height, m_area;

    unsigned int m_iHistorySize;
    PositionHistory m_PositionHistory;

    friend class FingerClient;
};

class UserInfo
{
  public:
    UserInfo(int id) : m_iUserId(id) {}
    int getUserId() { return m_iUserId; }
    std::set<int>& FingerIds() { return m_FingerIds; }

  private:
    int m_iUserId;
    std::set<int> m_FingerIds;

    friend class FingerClient;
};

#endif // FINGERINFO_H
