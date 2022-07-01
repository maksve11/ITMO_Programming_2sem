#include <iostream>
#include <cmath>
#include <vector>

class Point
{
private:
    double x_, y_;

public:
    explicit Point(double x = 0, double y = 0) : x_(x), y_(y) {}

    double getx() const
    {
        return x_;
    }

    double gety() const
    {
        return y_;
    }

    void setx(double x)
    {
        this->x_ = x;
    }

    void sety(double y)
    {
        this->y_ = y;
    }

    Point &operator=(const Point &other)
    {
        if (this == &other)
            return *this;
        setx(other.getx());
        sety(other.gety());
        return *this;
    }

    Point(const Point &other)
        : x_(other.x_), y_(other.y_) {}
};

class Broken
{
protected:
    std::vector<Point> points_;

public:
    explicit Broken(const std::vector<Point> &points) : points_(points)
    {
        if (points_.size() < 2)
            throw std::invalid_argument("too few points!");
    }

    Broken(const Broken &other) : points_(other.points_)
    {
    }

    Broken(const int &count)
    {
        points_.resize(count);
        if (points_.size() < 2)
            throw std::invalid_argument("too few points");
        else
        {
            Point t;
            for (int i = 0; i < count; i++)
                points_[i] = t;
        }
    }

    Broken &operator=(const Broken &other)
    {
        if (this == &other)
            return *this;
        points_ = other.points_;
        return *this;
    }

    int CountofPoints() const
    {
        return this->points_.size();
    }

    virtual double brokenlength() const
    {
        double l = 0;
        for (int i = 0; i < this->CountofPoints() - 1; i++)
        {
            l += sqrt(pow(this->points_[i].getx() - this->points_[i + 1].getx(), 2) + pow(this->points_[i].gety() - this->points_[i + 1].gety(), 2));
        }
        return l;
    }

    Point getp(int index) const
    {
        return points_[index];
    }

    Point back() const
    {
        return points_.back();
    }

    Point front() const
    {
        return points_.front();
    }

    ~Broken()
    {
        points_.clear();
    }
};

class Closed : public Broken
{
public:
    explicit Closed(const std::vector<Point> &points) : Broken(points)
    {
        if (points.size() < 3)
            throw std::invalid_argument("too few points");
    }

    Closed(const Broken &points) : Broken(points)
    {
        if (points.CountofPoints() < 3)
            throw std::invalid_argument("too few points");
    }

    double closedlenght() const
    {
        double l = Broken::brokenlength();
        l += sqrt(pow(this->points_.front().getx() - this->points_.back().getx(), 2) + pow(this->points_.front().gety() - this->points_.back().gety(), 2));
        return l;
    }
};

class Polygon
{
protected:
    Closed line;

public:
    bool convexmethod() const
    {
        bool t;
        if (!(((line.getp(1).getx() - line.front().getx()) * (line.getp(2).gety() - line.getp(1).gety()) -
               (line.getp(1).gety() - line.front().gety()) * (line.getp(2).getx() - line.getp(1).getx())) < 0))
            t = true;
        else
            t = false;
        for (int i = 1; i < line.CountofPoints() - 2; i++)
        {
            double v = (line.getp(i + 1).getx() - line.getp(i).getx()) * (line.getp(i + 2).gety() - line.getp(i + 1).gety()) - (line.getp(i + 1).gety() - line.getp(i).gety()) * (line.getp(i + 2).getx() - line.getp(i + 2).getx());
            if (v < 0 && t == true)
                return false;
            if (v > 0 && t == false)
                return false;
        }
        return true;
    }

    explicit Polygon(const std::vector<Point> &line) : line(line)
    {
        if (line.size() < 3)
            throw std::invalid_argument("too few points");
        if (!this->convexmethod())
            throw std::invalid_argument("not convex");
    }

    Polygon(const Polygon &p) : line(p.line)
    {
    }

    Polygon(const Closed &p) : line(p)
    {
        if (line.CountofPoints() < 3)
            throw std::invalid_argument("too few points");
    }

    Polygon &operator=(const Polygon &p)
    {
        if (&p == this)
            return *this;
        this->line = p.line;
        return *this;
    }

    int PolygonCount() const
    {
        return this->line.CountofPoints();
    }

    double perimeter() const
    {
        double p = this->line.closedlenght();
        return fabs(p);
    }

    virtual double square() const
    {
        double s, t = 0;
        for (int i = 0; i < line.CountofPoints() - 1; i++)
        {
            t += line.getp(i).getx() * line.getp(i + 1).gety();
            t -= line.getp(i + 1).getx() * line.getp(i).gety();
        }
        s = t / 2;
        return fabs(s);
    }
};

class Triangle : public Polygon
{
public:
    explicit Triangle(const std::vector<Point> &line) : Polygon(line)
    {
        if (line.size() != 3)
            throw std::invalid_argument("it is not a triangle");
        double s1 = (this->line.getp(2).getx() - this->line.front().getx()) * (this->line.getp(1).gety() - this->line.front().gety());
        double s2 = (this->line.getp(1).getx() - this->line.front().getx()) * (this->line.getp(2).gety() - this->line.front().gety());
        if (s1 == s2)
            throw std::invalid_argument("It is a line");
    }

    Triangle(const Closed &line) : Polygon(line)
    {
        if (line.CountofPoints() != 3)
            throw std::invalid_argument("it is not a triangle");
        double s1 = (this->line.getp(2).getx() - this->line.front().getx()) * (this->line.getp(1).gety() - this->line.front().gety());
        double s2 = (this->line.getp(1).getx() - this->line.front().getx()) * (this->line.getp(2).gety() - this->line.front().gety());
        if (s1 == s2)
            throw std::invalid_argument("It is a line");
    }

    double square() const override
    {
        double s = 0.5 * ((this->line.getp(1).getx() - this->line.front().getx()) * (this->line.getp(2).gety() - this->line.front().gety()) -
                          (this->line.getp(2).getx() - this->line.front().getx()) * (this->line.getp(1).gety() - this->line.front().gety()));
        return s;
    }
};

class Trapezoid : public Polygon
{
public:
    explicit Trapezoid(std::vector<Point> &line) : Polygon(line)
    {
        if (line.size() != 4)
            throw std::invalid_argument("it is not a trapezoid");
        double cos1 = ((line.front().getx() - line[1].getx()) * (line[2].getx() - line[3].getx()) + (line.front().gety() - line[1].gety()) * (line[2].gety() - line[3].gety())) / (sqrt(pow(line.front().getx() - line[1].getx(), 2) + pow(line.front().gety() - line[1].gety(), 2)) * sqrt(pow(line[2].getx() - line[3].getx(), 2) + pow(line[2].gety() - line[3].gety(), 2)));
        double cos2 = ((line[1].getx() - line[2].getx()) * (line[3].getx() - line.front().getx()) + (line[1].gety() - line[2].gety()) * (line[3].gety() - line.front().gety())) / (sqrt(pow(line[1].getx() - line[2].getx(), 2) + pow(line[3].gety() - line.front().gety(), 2)) * sqrt(pow(line[1].getx() - line[2].getx(), 2) + pow(line[3].gety() - line.front().gety(), 2)));
        if (!((std::abs(cos1) == 1 && std::abs(cos2) != 1) || (std::abs(cos1) != 1 && std::abs(cos2) == 1)))
            throw std::invalid_argument("twisted");
    }

    Trapezoid(const Closed &line) : Polygon(line)
    {
        if (line.CountofPoints() != 4)
            throw std::invalid_argument("it is not a trapezoid");
        double cos1 = ((line.front().getx() - line.getp(1).getx()) * (line.getp(2).getx() - line.getp(3).getx()) + (line.front().gety() - line.getp(1).gety()) * (line.getp(2).gety() - line.getp(3).gety())) / (sqrt(pow(line.front().getx() - line.getp(1).getx(), 2) + pow(line.front().gety() - line.getp(1).gety(), 2)) * sqrt(pow(line.getp(2).getx() - line.getp(3).getx(), 2) + pow(line.getp(2).gety() - line.getp(3).gety(), 2)));
        double cos2 = ((line.getp(1).getx() - line.getp(2).getx()) * (line.getp(3).getx() - line.front().getx()) + (line.getp(1).gety() - line.getp(2).gety()) * (line.getp(3).gety() - line.front().gety())) / (sqrt(pow(line.getp(1).getx() - line.getp(2).getx(), 2) + pow(line.getp(3).gety() - line.front().gety(), 2)) * sqrt(pow(line.getp(1).getx() - line.getp(2).getx(), 2) + pow(line.getp(3).gety() - line.front().gety(), 2)));
        if (!((std::abs(cos1) == 1 && std::abs(cos2) != 1) || (std::abs(cos1) != 1 && std::abs(cos2) == 1)))
            throw std::invalid_argument("twisted");
    }
};

class RegularPolygon : public Polygon
{
public:
    explicit RegularPolygon(std::vector<Point> &line) : Polygon(line)
    {
        double side = (sqrt(pow(line.front().getx() - line[line.size() - 1].getx(), 2) + pow(line.front().gety() - line[line.size() - 1].gety(), 2)));
        for (int i = 0; i < line.size() - 1; i++)
            if (sqrt(pow(line[i].getx() - line[i + 1].getx(), 2) + pow(line[i].gety() - line[i + 1].gety(), 2)) != side)
                throw std::invalid_argument("different sides");
    }

    RegularPolygon(const Closed &line) : Polygon(line)
    {
        double side = (sqrt(pow(line.front().getx() - line.getp(line.CountofPoints() - 1).getx(), 2) + pow(line.front().gety() - line.getp(line.CountofPoints() - 1).gety(), 2)));
        for (int i = 0; i < line.CountofPoints() - 1; i++)
            if (sqrt(pow(line.getp(i).getx() - line.getp(i + 1).getx(), 2) + pow(line.getp(i).gety() - line.getp(i + 1).gety(), 2)) != side)
                throw std::invalid_argument("different sides");
    }
};

int main()
{
    // Point
    // std::cout << "Point test\n";
    // Point point(1, 2);
    // std::cout << point.getx() << "\n";
    // // point.SetCord(-9, 5);
    // point.setx(-9);
    // point.sety(5);
    // std::cout << point.getx() << "\n";
    // Point point1(5, 6);
    // point = point1;
    // std::cout << point.getx() << "\n";
    // // Broken Line
    // std::cout << "Broken Line test\n";
    // std::vector<Point> s;
    // s[0] = point;
    // s[1] = point1;
    // Broken t(s);
    // std::cout << t.back().getx() << '\n';
    // std::cout << "Chain test\n";
    // Closed chain(s);
    // std::cout << chain.getp(2).getx() << "\n";
    // Polygon
    // Point p(3, 1);
    // Point p1(9, 1);
    // Point p2(6, 5);
    // Point p3(3, 5);
    // std::vector<Point> ss;
    // ss.push_back(p);
    // ss.push_back(p1);
    // ss.push_back(p2);
    // ss.push_back(p3);
    // Broken tt(ss);
    // std::cout << tt.CountofPoints() << '\n';
    // ss.push_back(p2);
    // ss.push_back(p3);
    // Closed brokenLine(ss);
    // for (int i = 0; i < brokenLine.CountofPoints(); i++)
    //     std::cout << brokenLine.getp(i).getx() << ' ' << brokenLine.getp(i).gety() << '\n';
    // std::cout << brokenLine.closedlenght() << "\n";
    // std::cout << "Polygon\n";
    // Polygon t(brokenLine);
    // std::cout << t.perimeter() << "\n";

    // // Triangle
    // std::cout << "Triangle test\n";

    // std::vector<Point> ss1;
    // ss1.push_back(p);
    // ss1.push_back(p1);
    // ss1.push_back(p2);
    // ss1.push_back(p3);
    // Closed brokenLine1(ss1);
    // Triangle triangle(brokenLine1);
    // std::cout << triangle.square() << " " << triangle.perimeter() << "\n";
    // // Trapezoid
    // std::cout << "Trapezoid test\n";
    // Trapezoid trap(brokenLine1);
    // std::cout << trap.square() << " " << trap.perimeter() << "\n";

    Point k(0, 0);
    Point k1(0, 1);
    Point k2(1, 1);
    Point k3(1, 0);
    Point k4(0, 0);

    std::vector<Point> ss2;
    ss2.push_back(k);
    ss2.push_back(k1);
    ss2.push_back(k2);
    ss2.push_back(k3);
    ss2.push_back(k4);
    Closed brokenLine2(ss2);
    // // RegularPolygon
    // std::cout << "Regular Polygon\n";
    Polygon regularPolygon(brokenLine2);
    // std::cout << brokenLine2.brokenlength() << '\n';
    std::cout << regularPolygon.square() << ' ' << regularPolygon.perimeter();
    return 0;
}