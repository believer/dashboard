let component = ReasonReact.statelessComponent("IconBook");

let make = (~className="", ~onClick, _children) => {
  ...component,
  render: _self =>
    <div className="hover-hot-pink pointer" onClick>
      <svg
        xmlns="http://www.w3.org/2000/svg"
        width="24"
        height="24"
        viewBox="0 0 24 24"
        fill="none"
        stroke="currentColor"
        strokeWidth="2"
        strokeLinecap="round"
        strokeLinejoin="round"
        className={Cn.make(["feather feather-book", className])}>
        <path d="M4 19.5A2.5 2.5 0 0 1 6.5 17H20" />
        <path
          d="M6.5 2H20v20H6.5A2.5 2.5 0 0 1 4 19.5v-15A2.5 2.5 0 0 1 6.5 2z"
        />
      </svg>
    </div>,
};
