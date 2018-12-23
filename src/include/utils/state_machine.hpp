#pragma once
#include <boost/mpl/list.hpp>
#include <boost/mpl/fold.hpp>  
#include <boost/mpl/filter_view.hpp>  
#include <type_traits>
#include <boost/mpl/vector.hpp>  
#include <boost/mpl/placeholders.hpp>  

// in order to use this - you would derived from the class state_machine and pass
// as the first template arg the class that derived from state_machine
// then you must define a constat value called inital_state that is as the name imply the 
// statrting stae for the state machine
// struct transition_table : mpl::vector<  
  
        //    Start     Event         Next      Action  
        //  +---------+-------------+---------+------------------+  
//        row < Stopped , play        , Playing , &start_playback  >,  
//        row < Stopped , open_close  , Open    , &open_drawer     >,  
        //  +---------+-------------+---------+------------------+  
//        row < Open    , open_close  , Empty   , &close_drawer    >,  
        //  +---------+-------------+---------+------------------+  
  //      row < Empty   , open_close  , Open    , &open_drawer     >,  
 //       row < Empty   , cd_detected , Stopped , &store_cd_info   >,  
        //  +---------+-------------+---------+------------------+  
  //      row < Playing , stop        , Stopped , &stop_playback   >,  
  //      row < Playing , pause       , Paused  , &pause_playback  >,  
  //      row < Playing , open_close  , Open    , &stop_and_open   >,  
        //  +---------+-------------+---------+------------------+  
  //      row < Paused  , play        , Playing , &resume_playback >,  
  //      row < Paused  , stop        , Stopped , &stop_playback   >,  
  //      row < Paused  , open_close  , Open    , &stop_and_open   >  
        //  +---------+-------------+---------+------------------+  
  
 //   > {};  
 //   where in this example we have the action as memober function
 //   and the name oof the class that we derived from is this 
 //   exsample is Plaer

template<typename Transition, class Next  >  
struct event_dispatcher  
{  //Finite State Machines  
    using fsm_t = typename Transition::fsm_t ;  
    using event = typename Transition::event ;  
              
    static int dispatch(fsm_t& fsm, int state, event const& e)  
    {  
        if (state == Transition::current_state)  {  
            Transition::execute(fsm, e);  
            return Transition::next_state;  
        } else { // move on to the next node in the chain.  
            return Next::dispatch(fsm, state, e);  
        }  
    }  
                  
};  

template <class Derived> class state_machine;

struct default_event_dispatcher  
{  
    template<typename FSM, typename Event>  
    static int dispatch(state_machine<FSM>& m, int state, Event const& e)  
    {  
        return m.call_no_transition(state, e);  
    }  
};  

template<class Table, class Event>  
struct generate_dispatcher;

template<class Derived>  
class state_machine  
{  
protected:  
    template<int CurrentState, typename Event, int NextState, void (*action)(Derived&, Event const&)>  
    struct row  
    {  
    //for later use by our metaprogram  
        enum { current_state = CurrentState };  
        enum { next_state = NextState };  
        using event = Event;  
        using fsm_t = Derived ;  
        static void execute(Derived& fsm, Event const& e)  
        {  
            (*action)(fsm, e);  
        }  

    };
    friend struct default_event_dispatcher;

   template <class Event>  
    int call_no_transition(int state, Event const& e)  
    {  
        return static_cast<Derived*>(this)->no_transition(state, e);  // CRTP downcast                                        
    }  

public:
        template<typename Event>
        int process_event(const Event& ev)
        {
            using dispatcher = typename generate_dispatcher<typename Derived::trasition_table, Event>::type;
            this->state = dispatcher::dispatch(*(static_cast<Derived*>(this)), this->state, ev);
            return this->state;
        }

protected:
        constexpr state_machine() = default;
        state_machine(const state_machine&) = default;
        state_machine& operator = (const state_machine&) = default;
        state_machine(state_machine&&) = default;
        state_machine& operator = (state_machine&&) = default;

        template<typename Event>
        int no_transition(int state, const Event& )
        {
            return state;
        }

private:
        int state = Derived::inital_state;
};

template <class Event>   
struct is_same_event  
{  
    template <typename Transition> 
    struct apply : std::is_same<Event, typename Transition::event>
    {
    };
};

template<typename Table, typename Event>  
struct generate_dispatcher : boost::mpl::fold<  
          boost::mpl::filter_view<Table, is_same_event<Event>>, default_event_dispatcher,
                                  event_dispatcher<boost::mpl::placeholders::_2, boost::mpl::placeholders::_1>
                            >  
{
};


