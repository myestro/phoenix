/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010-2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_ENVIRONMENT_HPP
#define PHOENIX_CORE_ENVIRONMENT_HPP

#include <boost/phoenix/core/limits.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/proto/transform/impl.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <typeinfo>

namespace boost { namespace phoenix 
{
    namespace result_of
    {
        template <typename Env, typename Actions>
        struct context
        {
            typedef fusion::vector2<Env, Actions> type;
        };

        template <typename Context>
        struct env
        {
            typedef
                typename fusion::result_of::at_c<
                    typename boost::remove_reference<Context>::type
                  , 0
                >::type
                type;
        };
        
        template <typename Context>
        struct actions
        {
            typedef
                typename fusion::result_of::at_c<
                    typename boost::remove_reference<Context>::type
                  , 1
                >::type
                type;
        };
    }

    namespace functional
    {
        struct context
        {
            BOOST_PROTO_CALLABLE()

            template <typename Sig>
            struct result;

            template <typename This, typename Env, typename Actions>
            struct result<This(Env, Actions)>
                : result<This(Env const &, Actions const &)>
            {};

            template <typename This, typename Env, typename Actions>
            struct result<This(Env &, Actions)>
                : result<This(Env &, Actions const &)>
            {};

            template <typename This, typename Env, typename Actions>
            struct result<This(Env, Actions &)>
                : result<This(Env const &, Actions &)>
            {};

            template <typename This, typename Env, typename Actions>
            struct result<This(Env &, Actions &)>
                : result_of::context<Env, Actions>
            {};

            template <typename Env, typename Actions>
            typename result_of::context<Env &, Actions &>::type
            operator()(Env & env, Actions & actions) const
            {
                return fusion::vector2<Env &, Actions &>(env, actions);
            }

            template <typename Env, typename Actions>
            typename result_of::context<Env const &, Actions &>::type
            operator()(Env const & env, Actions & actions) const
            {
                return fusion::vector2<Env const &, Actions &>(env, actions);
            }

            template <typename Env, typename Actions>
            typename result_of::context<Env &, Actions const &>::type
            operator()(Env & env, Actions const & actions) const
            {
                return fusion::vector2<Env &, Actions const &>(env, actions);
            }

            template <typename Env, typename Actions>
            typename result_of::context<Env const &, Actions const &>::type
            operator()(Env const & env, Actions const & actions) const
            {
                return fusion::vector2<Env const &, Actions const &>(env, actions);
            }
        };

        struct env
        {
            BOOST_PROTO_CALLABLE()

            template <typename Sig>
            struct result;

            template <typename This, typename Context>
            struct result<This(Context)>
                : result<This(Context const &)>
            {};

            template <typename This, typename Context>
            struct result<This(Context &)>
                : result_of::env<Context>
            {};

            template <typename Context>
            typename result_of::env<Context const>::type
            operator()(Context const & ctx) const
            {
                return fusion::at_c<0>(ctx);
            }

            template <typename Context>
            typename result_of::env<Context>::type
            operator()(Context & ctx) const
            {
                return fusion::at_c<0>(ctx);
            }
        };
        
        struct actions
        {
            BOOST_PROTO_CALLABLE()

            template <typename Sig>
            struct result;

            template <typename This, typename Context>
            struct result<This(Context)>
                : result<This(Context const &)>
            {};

            template <typename This, typename Context>
            struct result<This(Context &)>
                : result_of::actions<Context>
            {};

            template <typename Context>
            typename result_of::actions<Context const>::type
            operator()(Context const & ctx) const
            {
                return fusion::at_c<1>(ctx);
            }

            template <typename Context>
            typename result_of::actions<Context>::type
            operator()(Context & ctx) const
            {
                return fusion::at_c<1>(ctx);
            }
        };

    }

    struct _context
        : proto::transform<_context>
    {
        template <typename Expr, typename State, typename Data>
        struct impl
            : proto::transform_impl<Expr, State, Data>
        {
            typedef fusion::vector2<State, Data> result_type;

            result_type operator()(
                typename impl::expr_param
              , typename impl::state_param s
              , typename impl::data_param d
            ) const
            {
                return fusion::vector2<State, Data>(s, d);
            }
        };
    };

    template <typename Env, typename Actions>
    typename result_of::context<Env, Actions>::type
    context(Env const& env, Actions const& actions)
    {
        return fusion::vector2<Env, Actions>(env, actions);
    }

    struct _env
        : proto::transform<_env>
    {
        template <typename Expr, typename State, typename Data>
        struct impl
            : proto::transform_impl<Expr, State, Data>
        {
            typedef State result_type;

            result_type operator()(
                typename impl::expr_param
              , typename impl::state_param s
              , typename impl::data_param
            ) const
            {
                return s;
            }
        };
    };

    template <typename Expr, typename State>
    struct _env::impl<Expr, State, int>
        : proto::transform_impl<Expr, State, int>
    {
            typedef
                typename fusion::result_of::at_c<
                    typename boost::remove_reference<State>::type
                  , 0
                >::type
                result_type;

            result_type operator()(
                typename impl::expr_param
              , typename impl::state_param s
              , typename impl::data_param
            ) const
            {
                return fusion::at_c<0>(s);
            }
    };

    template <typename Context>
    typename fusion::result_of::at_c<Context, 0>::type
    env(Context & ctx)
    {
        return fusion::at_c<0>(ctx);
    }

    struct _actions
        : proto::transform<_actions>
    {
        template <typename Expr, typename State, typename Data>
        struct impl
            : proto::transform_impl<Expr, State, Data>
        {
            typedef Data result_type;

            result_type operator()(
                typename impl::expr_param
              , typename impl::state_param
              , typename impl::data_param d
            ) const
            {
                return d;
            }
        };
    };

    template <typename Expr, typename State>
    struct _actions::impl<Expr, State, int>
        : proto::transform_impl<Expr, State, int>
    {
            typedef
                typename fusion::result_of::at_c<
                    typename boost::remove_reference<State>::type
                  , 1
                >::type
                result_type;

            result_type operator()(
                typename impl::expr_param
              , typename impl::state_param s
              , typename impl::data_param
            ) const
            {
                return fusion::at_c<1>(s);
            }
    };

    template <typename Context>
    typename fusion::result_of::at_c<Context, 1>::type
    actions(Context & ctx)
    {
        return fusion::at_c<1>(ctx);
    }

    template <typename T, typename Enable = void>
    struct is_environment : fusion::traits::is_sequence<T> {};
}}

#endif

