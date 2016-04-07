#ifndef _LIB_JSON_H_
#define _LIB_JSON_H_

#include <gmpxx.h>

#include <iostream>
#include <vector>
#include <stdexcept>

#include "lib/cstring.h"
#include "lib/map.h"

namespace Test { class TestJson; }

namespace Util {

class IJson {
 public:
    virtual void serialize(std::ostream& out) const = 0;
    cstring toString() const;
    template<typename T> bool is() const { return to<T>() != nullptr; }
    template<typename T> T* to() { return dynamic_cast<T*>(this); }
    template<typename T> const T* to() const { return dynamic_cast<const T*>(this); }
};

class JsonValue final : public IJson {
    friend class Test::TestJson;
 public:
    enum Kind {
        String,
        Number,
        True,
        False,
        Null
    };
    JsonValue() : tag(Kind::Null) {}
    JsonValue(bool b) : tag(b ? Kind::True : Kind::False) {}  // NOLINT
    JsonValue(mpz_class v) : tag(Kind::Number), value(v) {}   // NOLINT
    JsonValue(int v) : tag(Kind::Number), value(v) {}         // NOLINT
    JsonValue(long v) : tag(Kind::Number), value(v) {}        // NOLINT
    JsonValue(unsigned v) : tag(Kind::Number), value(v) {}    // NOLINT
    JsonValue(unsigned long v) : tag(Kind::Number), value(v) {}// NOLINT
    JsonValue(double v) : tag(Kind::Number), value(v) {}      // NOLINT
    JsonValue(float v) : tag(Kind::Number), value(v) {}       // NOLINT
    JsonValue(cstring s) : tag(Kind::String), str(s) {}       // NOLINT
    JsonValue(std::string s) : tag(Kind::String), str(s) {}   // NOLINT
    JsonValue(const char* s) : tag(Kind::String), str(s) {}   // NOLINT
    void serialize(std::ostream& out) const;

    bool operator==(const bool& b) const;
    bool operator==(const mpz_class& v) const;
    bool operator==(const int& v) const;
    bool operator==(const long& v) const;
    bool operator==(const unsigned& v) const;
    bool operator==(const unsigned long& v) const;
    bool operator==(const double& v) const;
    bool operator==(const float& v) const;
    bool operator==(const cstring& s) const;
    bool operator==(const std::string& s) const;
    bool operator==(const char* s) const;
    bool operator==(const JsonValue& other) const;

    bool isNumber() const { return tag == Kind::Number; }
    bool isBool() const { return tag == Kind::True || tag == Kind::False; }
    bool isString() const { return tag == Kind::String; }
    bool isNull() const { return tag == Kind::Null; }

    bool getBool() const;
    cstring getString() const;
    mpz_class getValue() const;

    static JsonValue* null;

 private:
    JsonValue(Kind kind) : tag(kind) {                        // NOLINT
        if (kind == Kind::String || kind == Kind::Number)
            throw std::logic_error("Incorrect constructor called");
    }

    const Kind tag;
    const mpz_class value = 0;
    const cstring str = nullptr;
};

class JsonArray final : public IJson {
    friend class Test::TestJson;
 public:
    std::vector<const IJson*> values;
    void serialize(std::ostream& out) const;
    JsonArray* append(const IJson* value);
    JsonArray* append(bool b) { append(new JsonValue(b)); return this; }
    JsonArray* append(mpz_class v) { append(new JsonValue(v)); return this; }
    JsonArray* append(int v) { append(new JsonValue(v)); return this; }
    JsonArray* append(long v) { append(new JsonValue(v)); return this; }
    JsonArray* append(unsigned v) { append(new JsonValue(v)); return this; }
    JsonArray* append(unsigned long v) { append(new JsonValue(v)); return this; }
    JsonArray* append(double v) { append(new JsonValue(v)); return this; }
    JsonArray* append(float v) { append(new JsonValue(v)); return this; }
    JsonArray* append(cstring s) { append(new JsonValue(s)); return this; }
    JsonArray* append(std::string s) { append(new JsonValue(s)); return this; }
    JsonArray* append(const char* s) { append(new JsonValue(s)); return this; }
    JsonArray(std::initializer_list<const IJson*> data) : values(data) {} // NOLINT
    JsonArray() = default;
};

class JsonObject final : public IJson {
    friend class Test::TestJson;
    std::map<cstring, const IJson*> values;
    std::vector<cstring> labelOrder;

 public:
    JsonObject() = default;
    void serialize(std::ostream& out) const;
    JsonObject* emplace(cstring label, const IJson* value);
    JsonObject* emplace(cstring label, bool b)
    { emplace(label, new JsonValue(b)); return this; }
    JsonObject* emplace(cstring label, mpz_class v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, int v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, long v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, unsigned v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, unsigned long v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, double v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, float v)
    { emplace(label, new JsonValue(v)); return this; }
    JsonObject* emplace(cstring label, cstring s)
    { emplace(label, new JsonValue(s)); return this; }
    JsonObject* emplace(cstring label, std::string s)
    { emplace(label, new JsonValue(s)); return this; }
    JsonObject* emplace(cstring label, const char* s)
    { emplace(label, new JsonValue(s)); return this; }
    const IJson* get(cstring label) const { return ::get(values, label); }
};

}  // namespace Util

#endif  /* _LIB_JSON_H_ */