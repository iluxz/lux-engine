#pragma once

#include "lux/Core.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>
#include <set>

namespace lux {

// Entity ID
using Entity = u32;
constexpr Entity NULL_ENTITY = 0;
constexpr Entity MAX_ENTITIES = 100000;

// Component base
struct IComponent {
    virtual ~IComponent() = default;
};

// Component pool base
struct IComponentPool {
    virtual ~IComponentPool() = default;
    virtual void remove(Entity entity) = 0;
};

// Component pool
template<typename T>
class ComponentPool : public IComponentPool {
public:
    void add(Entity entity, T component) {
        if (m_entityToIndex.find(entity) == m_entityToIndex.end()) {
            m_entityToIndex[entity] = m_components.size();
            m_indexToEntity.push_back(entity);
            m_components.push_back(std::move(component));
        } else {
            m_components[m_entityToIndex[entity]] = std::move(component);
        }
    }

    void remove(Entity entity) override {
        auto it = m_entityToIndex.find(entity);
        if (it == m_entityToIndex.end()) return;

        size_t index = it->second;
        size_t lastIndex = m_components.size() - 1;

        // Swap with last
        if (index != lastIndex) {
            m_components[index] = std::move(m_components[lastIndex]);
            m_indexToEntity[index] = m_indexToEntity[lastIndex];
            m_entityToIndex[m_indexToEntity[index]] = index;
        }

        m_components.pop_back();
        m_indexToEntity.pop_back();
        m_entityToIndex.erase(entity);
    }

    T* get(Entity entity) {
        auto it = m_entityToIndex.find(entity);
        if (it == m_entityToIndex.end()) return nullptr;
        return &m_components[it->second];
    }

    bool has(Entity entity) const {
        return m_entityToIndex.find(entity) != m_entityToIndex.end();
    }

    const std::vector<T>& getAll() const { return m_components; }
    const std::vector<Entity>& getEntities() const { return m_indexToEntity; }
    size_t size() const { return m_components.size(); }

private:
    std::vector<T> m_components;
    std::vector<Entity> m_indexToEntity;
    std::unordered_map<Entity, size_t> m_entityToIndex;
};

// Registry
class Registry {
public:
    Registry() {
        m_entityCounter = 1; // 0 is null
    }

    Entity createEntity() {
        Entity entity;
        if (!m_freeEntities.empty()) {
            entity = m_freeEntities.back();
            m_freeEntities.pop_back();
        } else {
            entity = m_entityCounter++;
        }
        m_aliveEntities.insert(entity);
        return entity;
    }

    void destroyEntity(Entity entity) {
        m_aliveEntities.erase(entity);
        m_freeEntities.push_back(entity);
        // Remove all components
        for (auto& [type, pool] : m_pools) {
            pool->remove(entity);
        }
    }

    template<typename T>
    void addComponent(Entity entity, T component = T{}) {
        getPool<T>().add(entity, std::move(component));
    }

    template<typename T>
    void removeComponent(Entity entity) {
        getPool<T>().remove(entity);
    }

    template<typename T>
    T* getComponent(Entity entity) {
        return getPool<T>().get(entity);
    }

    template<typename T>
    bool hasComponent(Entity entity) {
        return getPool<T>().has(entity);
    }

    template<typename T>
    ComponentPool<T>& getPool() {
        auto type = std::type_index(typeid(T));
        if (m_pools.find(type) == m_pools.end()) {
            m_pools[type] = std::make_unique<ComponentPool<T>>();
        }
        return *static_cast<ComponentPool<T>*>(m_pools[type].get());
    }

    template<typename T>
    void view(std::function<void(Entity, T&)> func) {
        auto& pool = getPool<T>();
        for (size_t i = 0; i < pool.size(); ++i) {
            func(pool.getEntities()[i], pool.getAll()[i]);
        }
    }

    template<typename T1, typename T2>
    void view(std::function<void(Entity, T1&, T2&)> func) {
        auto& pool1 = getPool<T1>();
        for (Entity entity : pool1.getEntities()) {
            if (getPool<T2>().has(entity)) {
                func(entity, *pool1.get(entity), *getPool<T2>().get(entity));
            }
        }
    }

    const std::set<Entity>& getAliveEntities() const { return m_aliveEntities; }

private:
    Entity m_entityCounter;
    std::vector<Entity> m_freeEntities;
    std::set<Entity> m_aliveEntities;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pools;
};

} // namespace lux