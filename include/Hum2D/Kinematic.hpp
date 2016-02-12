#ifndef H2D_KINEMATIC_HPP
#define H2D_KINEMATIC_HPP
#include "Behaviour.hpp"

namespace h2d
{
class Time;
class Transformation;
class Kinematic : public Behaviour
{
public:
    Kinematic();
    virtual ~Kinematic();

    virtual void init() override;
    virtual void onDestroy() override;
    virtual Transformation simulate(const Time& delta_time) const;

    Transformation& velocity();
    const Transformation& velocity() const;
    Transformation& acceleration();
    const Transformation& acceleration() const;

    static const char* behaviourName();

private:
    Transformation* p_velocity;
    Transformation* p_acceleration;
};
}
#endif
