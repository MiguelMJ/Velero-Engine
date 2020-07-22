#include "EventSystem.hpp"

using namespace ge::EventSystem::_private_;
namespace ge{
    namespace EventSystem{
        namespace _private_{
            std::vector<channel> g_channels;
        }
        std::vector< std::set<EventListener*> > g_subscribers;
        std::map<std::string, size_t> g_channelIndex;
        std::queue<size_t> g_freeChannels;
        // "public"
        void registerListener(EventListener* el){
            for(auto& strch: el->m_channels){
                g_subscribers[getChannelId(strch)].insert(el);;
            }
        }
        void unregisterListener(EventListener* el){
            for(auto& strch: el->m_channels){
                g_subscribers[getChannelId(strch)].erase(el);;
            }
        }
        void subscribe(size_t ch, EventListener* el){
            g_subscribers[ch].insert(el);
        }
        void unsubscribe(size_t ch, EventListener* el){
            g_subscribers[ch].erase(el);
        }
        size_t getChannelId (const std::string& str){
            size_t ret;
            auto it = g_channelIndex.find(str);
            if(it != g_channelIndex.end()){ // already exist
                ret = it->second;
            }else if(g_freeChannels.empty()){ // there are no free channels
                ret = g_channels.size();
                g_channels.emplace_back();
                g_subscribers.emplace_back();
                g_channelIndex[str]=ret;
            }else{ // there are free channels
                ret = g_freeChannels.front();
                g_freeChannels.pop();
            }
            return ret;
        }
        bool freeChannel(const std::string& str){
            size_t ch = getChannelId(str);
            while (!g_channels.empty()) g_channels[ch].pop();
            g_subscribers[ch].clear();
            g_freeChannels.push(ch);
            g_channelIndex.erase(g_channelIndex.find(str));
        }
        std::set<constevptr> deletable;
        void dispatch(){
            for(int i=0; i < g_channels.size(); i++){
                dispatch(i);
            }
        }
        void dispatch(size_t ch){
            auto& q = g_channels[ch];
            while(!q.empty()){
                auto& ind_ev = q.front();
                for(auto& el: g_subscribers[ch]){
                    el->notify(ind_ev.second, ind_ev.first, ch);
                }
                q.pop();
            }
        }
        
    }
}
