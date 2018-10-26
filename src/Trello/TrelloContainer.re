type state =
  | Error(string)
  | Loading
  | Loaded(array(Trello.notification));

type action =
  | MarkAllAsRead
  | MarkAsRead(string)
  | NotificationsError(string)
  | NotificationsFetch
  | NotificationsFetched(array(Trello.notification));

let component = ReasonReact.reducerComponent("TrelloContainer");

let make = _children => {
  ...component,

  didMount: self => {
    self.send(NotificationsFetch);

    let intervalId =
      Js.Global.setInterval(() => self.send(NotificationsFetch), 1000 * 60);
    self.onUnmount(() => Js.Global.clearInterval(intervalId));
  },

  initialState: () => Loading,

  reducer: (action: action, state: state) =>
    switch (action) {
    | NotificationsError(err) => ReasonReact.Update(Error(err))
    | NotificationsFetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Trello.getNotifications()
              |> then_(notifications => {
                   DocumentTitle.updateTitleWithNotifications(
                     GitHub.Config.numberOfNotifications,
                     Trello.Config.numberOfNotifications,
                   );
                   self.send(NotificationsFetched(notifications)) |> resolve;
                 })
              |> catch(_ => self.send(NotificationsError("err")) |> resolve)
              |> ignore
            )
        ),
      )
    | NotificationsFetched(notifications) =>
      ReasonReact.Update(Loaded(notifications))
    | MarkAllAsRead =>
      ReasonReact.UpdateWithSideEffects(
        switch (state) {
        | Loaded(_) =>
          DocumentTitle.updateTitleWithNotifications(
            GitHub.Config.numberOfNotifications,
            "0",
          );

          Trello.Config.setNumberOfNotifications([||]);
          Loaded([||]);
        | _ => Loading
        },
        (
          _self =>
            Js.Promise.(
              Trello.markAllNotificationsAsRead()
              |> then_(_ => true |> resolve)
              |> ignore
            )
        ),
      )
    | MarkAsRead(id) =>
      ReasonReact.UpdateWithSideEffects(
        switch (state) {
        | Loaded(state) =>
          let filteredNotifications =
            state
            |> Js.Array.filter((item: Trello.notification) => item.id !== id);

          DocumentTitle.updateTitleWithNotifications(
            GitHub.Config.numberOfNotifications,
            Array.length(filteredNotifications) |> string_of_int,
          );

          Trello.Config.setNumberOfNotifications(filteredNotifications);
          Loaded(filteredNotifications);
        | _ => Loading
        },
        (
          _self =>
            Js.Promise.(
              Trello.markNotificationAsRead(id)
              |> then_(_ => true |> resolve)
              |> ignore
            )
        ),
      )
    },

  render: ({send, state}) =>
    <div className="w-100 w-50-l">
      <Header
        color="b--light-red"
        hasItems={
          switch (state) {
          | Loaded(n) => Array.length(n) > 0
          | _ => false
          }
        }
        markAllAsRead={_ => send(MarkAllAsRead)}
        title="Trello"
      />
      <Card>
        {
          Trello.Config.hasConfig ?
            switch (state) {
            | Error(_) => <CardError />
            | Loading => <CardLoading />
            | Loaded(notifications) =>
              let groupedByDate = Utils.groupTrelloByDate(notifications);

              Array.length(notifications) > 0 ?
                Js.Dict.values(groupedByDate)
                |> Js.Array.mapi((list, i) =>
                     <ListByDate
                       index=i
                       key={Js.Dict.keys(groupedByDate)[i]}
                       title={Js.Dict.keys(groupedByDate)[i]}
                       totalItems={
                         Array.length(Js.Dict.values(groupedByDate))
                       }>
                       {
                         list
                         |> Array.mapi((i, item: Trello.notification) =>
                              <TrelloNotification
                                item
                                isLast={Array.length(list) - 1 === i}
                                key={item.id}
                                markAsRead=(_ => send(MarkAsRead(item.id)))
                              />
                            )
                         |> ReasonReact.array
                       }
                     </ListByDate>
                   )
                |> ReasonReact.array :
                <EmptyState />;
            } :
            <TrelloMissing />
        }
      </Card>
    </div>,
};
