#include "Game.hpp"
#include "Actor.hpp"
#include "Plugin.hpp"
#include "Clock.hpp"

using namespace h2d;

Game::Game(unsigned int fixed_tickrate):
p_running(false),
c_nanoseconds_per_fixed_update(1e9 / (double)fixed_tickrate),
p_fixed_update_lag(0.0)
{};

Game::~Game()
{};

void Game::run()
{
    p_running = true;
    for (Plugin* p : p_plugins)
        p->gameStart();

    Clock clk;
    p_fixed_update_lag = 0.0;
    while (p_running)
    {
        p_delta_time = clk.reset();
        p_fixed_update_lag += p_delta_time.asNanoseconds();

        for (Actor* a : p_actors)
            a->preUpdate();
        for (Plugin* p : p_plugins)
            p->preUpdate();

        while (p_fixed_update_lag >= c_nanoseconds_per_fixed_update)
        {
            for (Plugin* p : p_plugins)
                p->preFixedUpdate();
            for (Actor* a : p_actors)
                a->fixedUpdate();
            for (Plugin* p : p_plugins)
                p->postFixedUpdate();
            p_fixed_update_lag -= c_nanoseconds_per_fixed_update;
        }

        for (Plugin* p : p_plugins)
            p->postUpdate();

        while (not p_actors_to_destroy.empty())
        {
            Actor* actor = *p_actors_to_destroy.begin();
            auto pool_it = p_actor_pool.find(actor);
            const std::list<Actor*>::iterator it = pool_it->second;
            actor->onDestroy();
            delete actor;
            p_actors.erase(it);
            p_actor_pool.erase(pool_it);
            p_actors_to_destroy.erase(p_actors_to_destroy.begin());
        }
    }

    for (Plugin* p : p_plugins)
        p->gameEnd();

    while (not p_actors.empty())
    {
        Actor* a = p_actors.front();
        auto pool_it = p_actor_pool.find(a);
        a->onDestroy();
        p_actor_pool.erase(pool_it);
        p_actors.pop_front();
        delete a;
    }
}

void Game::destroy(Actor& actor)
{
    p_actors_to_destroy.insert(&actor);
}

Actor& Game::makeActor()
{
    Actor* a = new Actor(*this);
    p_actors.push_back(a);
    p_actor_pool[a] = --p_actors.end();
    return *a;
}

void Game::addPlugin(Plugin& plugin)
{
    plugin.setGame(*this);
    p_plugins.push_back(&plugin);
}

const std::list<Actor*>& Game::actors() const
{
    return p_actors;
}

const Time& Game::deltaTime() const
{
    return p_delta_time;
}

Time Game::fixedUpdateTime() const
{
    return Time::nanoseconds(c_nanoseconds_per_fixed_update);
}

Time Game::fixedUpdateLag() const
{
    return Time::nanoseconds(p_fixed_update_lag);
}

void Game::setRunning(bool running)
{
    p_running = running;
}
