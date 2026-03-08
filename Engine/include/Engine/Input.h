#pragma once
/**
 * @file Input.h
 * @brief Flexible input system with context stack, action/axis bindings and rebinding.
 * @ingroup Engine
 */

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <array>

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Platform-agnostic backend interface for raw input events.
 *
 * Implement this interface to support a new windowing library.
 * The engine ships with a GLFW backend created via CreateGLFWInput().
 */
class IInputBackend
{
public:
    virtual ~IInputBackend() = default;

    /// @brief Polls the OS event queue (called once per frame by Input::Update()).
    virtual void PollEvents() = 0;

    /**
     * @brief Returns true if @p key is currently held down.
     * @param key  Platform-specific key code (e.g. GLFW_KEY_W).
     */
    virtual bool IsKeyDown(int key) const = 0;

    /**
     * @brief Returns true if @p button is currently held down.
     * @param button  Mouse button index (0 = left, 1 = right, 2 = middle).
     */
    virtual bool IsMouseButtonDown(int button) const = 0;

    /**
     * @brief Retrieves the current cursor position in screen pixels.
     * @param[out] x  Horizontal position.
     * @param[out] y  Vertical position.
     */
    virtual void GetMousePosition(double& x, double& y) const = 0;

    /// @brief Returns and clears the accumulated horizontal scroll delta.
    virtual float ConsumeScrollX() = 0;

    /// @brief Returns and clears the accumulated vertical scroll delta.
    virtual float ConsumeScrollY() = 0;
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief High-level input system with named bindings and a context stack.
 *
 * Features:
 *  - **Actions** – digital (pressed / held / released) named inputs.
 *  - **Axes**    – analogue values derived from key hold state and a scale factor.
 *  - **Contexts** – groups of bindings pushed/popped like a stack so that, e.g.,
 *    a menu context can shadow gameplay bindings without replacing them.
 *  - **Rebinding** – any binding can be re-mapped at runtime via Context::Rebind().
 *  - **Serialisation** – bindings can be saved/loaded via Context::Serialize() /
 *    Deserialize().
 *
 * Usage example:
 * @code
 *  auto ctx = input->CreateContext();
 *  ctx->BindAction(GLFW_KEY_SPACE, "Jump");
 *  ctx->BindAxis(GLFW_KEY_D, "MoveRight", 1.f);
 *  input->PushContext(ctx);
 *  // … per frame …
 *  if (input->ActionPressed("Jump")) player.Jump();
 *  float dx = input->Axis("MoveRight");
 * @endcode
 */
class Input
{
public:
    /**
     * @brief Constructs the Input system with the given backend.
     * @param backend  Platform-specific input backend (ownership transferred).
     */
    explicit Input(std::unique_ptr<IInputBackend> backend);
    ~Input() = default;

    /**
     * @brief Polls the backend, updates all key/mouse states and resolves bindings.
     * Must be called once per frame before any query methods.
     */
    void Update();

    // ── Binding types ──────────────────────────────────────────────────────

    /// @brief Distinguishes between digital (Action) and analogue (Axis) bindings.
    enum class BindingType { Action, Axis };

    /// @brief Describes a single key→name mapping.
    struct Binding
    {
        int         key;     ///< Raw key/button code.
        std::string name;    ///< Logical binding name, e.g. "Jump".
        BindingType type;    ///< Action or Axis.
        float       scale  = 1.f;  ///< Axis scale factor (ignored for actions).
        bool        consume = true; ///< If true, consuming the action clears the pressed state.
    };

    // ── Context ────────────────────────────────────────────────────────────

    /**
     * @brief A named group of key bindings that can be pushed onto the context stack.
     *
     * Multiple contexts can be active simultaneously; the top of the stack
     * takes priority.  Bindings in lower contexts are shadowed only if a
     * higher context defines the same logical name.
     */
    class Context
    {
    public:
        /**
         * @brief Adds a digital (action) binding.
         * @param key      Raw key/button code.
         * @param name     Logical action name.
         * @param consume  If true the pressed state is consumed after being read.
         */
        void BindAction(int key, const std::string& name, bool consume = true);

        /**
         * @brief Adds an analogue (axis) binding.
         * @param key    Raw key/button code.
         * @param name   Logical axis name.
         * @param scale  Contribution of this key when held (positive or negative).
         */
        void BindAxis(int key, const std::string& name, float scale);

        /**
         * @brief Changes the key for an existing binding identified by @p name.
         * @param name    Logical binding name to find.
         * @param newKey  Replacement key code.
         */
        void Rebind(const std::string& name, int newKey);

        /// @brief Serialisable representation of a single binding.
        struct SerializableBinding
        {
            std::string name;    ///< Logical name.
            int         key;     ///< Key code.
            BindingType type;    ///< Action or Axis.
            float       scale;   ///< Axis scale.
            bool        consume; ///< Consumption flag.
        };

        /// @brief Serialises all bindings in this context to a flat vector.
        std::vector<SerializableBinding> Serialize() const;

        /// @brief Restores bindings from a previously serialised vector.
        void Deserialize(const std::vector<SerializableBinding>& data);

    private:
        friend class Input;
        std::vector<Binding> m_bindings; ///< All bindings in this context.
    };

    // ── Context stack API ──────────────────────────────────────────────────

    /**
     * @brief Allocates and returns a new (empty) Context.
     */
    std::shared_ptr<Context> CreateContext();

    /**
     * @brief Pushes @p ctx onto the context stack (becomes active immediately).
     * @param ctx  Context to activate.
     */
    void PushContext(const std::shared_ptr<Context>& ctx);

    /// @brief Pops the top context from the stack.
    void PopContext();

    // ── Query API ──────────────────────────────────────────────────────────

    /**
     * @brief Returns true while the named action key is held down.
     * @param name  Logical action name.
     */
    bool Action(const std::string& name) const;

    /**
     * @brief Returns true for exactly one frame when the action key is first pressed.
     * @param name  Logical action name.
     */
    bool ActionPressed(const std::string& name) const;

    /**
     * @brief Returns true for exactly one frame when the action key is released.
     * @param name  Logical action name.
     */
    bool ActionReleased(const std::string& name) const;

    /**
     * @brief Returns the current value of an analogue axis (sum of all active bindings).
     * @param name  Logical axis name.
     */
    float Axis(const std::string& name) const;

    float MouseDX() const { return m_mouseDX; } ///< Horizontal mouse delta this frame (pixels).
    float MouseDY() const { return m_mouseDY; } ///< Vertical mouse delta this frame (pixels).
    float ScrollX() const { return m_scrollX; } ///< Horizontal scroll delta this frame.
    float ScrollY() const { return m_scrollY; } ///< Vertical scroll delta this frame.

    /**
     * @brief Returns the key code of any key pressed this frame, or -1 if none.
     */
    int GetAnyPressedKey() const;

    /// @brief Offset added to mouse button codes so they don't clash with keyboard codes.
    static constexpr int MOUSE_OFFSET   = 1000;
    /// @brief Offset added to gamepad button codes.
    static constexpr int GAMEPAD_OFFSET = 2000;

private:
    enum class KeyState { Up, Pressed, Down, Released };

    void PollKeyboard();
    void PollMouseButtons();
    void PollMouse();
    void ResolveContexts();

    std::unique_ptr<IInputBackend> m_backend;

    static constexpr int KEY_COUNT          = 349;
    static constexpr int MOUSE_BUTTON_COUNT = 8;

    std::array<KeyState, KEY_COUNT>          m_keys;
    std::array<KeyState, MOUSE_BUTTON_COUNT> m_mouseButtons;

    std::unordered_map<std::string, bool>  m_actionDown;
    std::unordered_map<std::string, bool>  m_actionPressed;
    std::unordered_map<std::string, bool>  m_actionReleased;
    std::unordered_map<std::string, float> m_axes;

    std::vector<std::shared_ptr<Context>> m_contexts;

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
    float  m_mouseDX    = 0.f;
    float  m_mouseDY    = 0.f;
    bool   m_firstMouse = true;
    float  m_scrollX    = 0.f;
    float  m_scrollY    = 0.f;
};

// ─────────────────────────────────────────────────────────────────────────────

struct GLFWwindow;

/**
 * @brief Factory function that creates a GLFW-backed Input system.
 * @param window  The GLFW window whose events should be processed.
 * @return Owning pointer to the configured Input instance.
 */
std::unique_ptr<Input> CreateGLFWInput(GLFWwindow* window);
