#ifndef H2D_ACTOR_HPP
#define H2D_ACTOR_HPP
#include <memory>
#include <list>
#include "Exceptions.hpp"

namespace h2d
{
class Game;
class Behaviour;
class Transformation;
class Actor
{
    friend Game;
public:
    ~Actor();
    void preUpdate();
    void fixedUpdate();
    void onDestroy();
    Game& game();
    const Game& game() const;
    unsigned int id() const;
    Transformation& transform();
    const Transformation& transform() const;
    template <typename B, class... Args>
    B& addBehaviour(Args&& ... args)
    {
        B* b = new B(args...);
        b->p_actor = this;
        p_asleep_behaviours.push_back(std::unique_ptr<Behaviour>(b));
        return *b;
    }
    template <typename T>
    T& getBehaviour()
    {
        for (std::unique_ptr<Behaviour>& b : p_behaviours)
            if (dynamic_cast<T*>(b.get()))
                return *dynamic_cast<T*>(b.get());
        throw exception::BehaviourNotFound(T::behaviourName());
    }
    template <typename T>
    const T& getBehaviour() const
    {
        for (const std::unique_ptr<Behaviour>& b : p_behaviours)
            if (dynamic_cast<T*>(b.get()))
                return *dynamic_cast<T*>(b.get());
        throw exception::BehaviourNotFound(T::behaviourName());
    }
    template <typename T>
    std::list<T*> getBehaviours()
    {
        std::list<T*> v;
        for (std::unique_ptr<Behaviour>& b : p_behaviours)
            if (dynamic_cast<T*>(b.get()))
                v.push_back(dynamic_cast<T*>(b.get()));
        return v;
    }
    template <typename T>
    std::list<const T*> getBehaviours() const
    {
        std::list<const T*> v;
        for (const std::unique_ptr<Behaviour>& b : p_behaviours)
            if (dynamic_cast<T*>(b.get()))
                v.push_back(dynamic_cast<T*>(b.get()));
        return v;
    }

private:
    Actor(Game& game, unsigned int id);

    Game* p_game;
    Transformation* p_transform;
    const unsigned int p_id;
    std::list<std::unique_ptr<Behaviour>> p_behaviours;
    std::list<std::unique_ptr<Behaviour>> p_asleep_behaviours;
};
}
#endif
